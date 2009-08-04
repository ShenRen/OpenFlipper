# This module provides the following macro:
#
# openflipper_plugin ( [DIRS dir1 dir2 ...]
#                      [DEPS dep1 dep2 ...]
#                      [OPTDEPS dep1 dep2 ...]
#                      [LDFLAGSADD flag1 flag2 ...]
#                      [CFLAGSADD flag1 flag2 ...]
#                      [LIBRARIES lib1 lib2 ...]
#                      [LIBDIRS dir1 dir2 ...]
#                      [INCDIRS dir1 dir2 ...]
#                      [ADDSRC file1 file2 ...]
#                      [INSTALLDATA dir1 dir2 ...]
#                      [TRANSLATION_LANGUAGES lang1 lang2 ...]
#                      [TRANSLATION_ADDFILES file1 file2 ...]
#                      [LICENSEMANAGER])
#
# DIRS        = additional directories with source files
# DEPS        = required dependencies for find_package macro
# OPTDEPS     = optional dependencies for find_package macro
# LDFLAGSADD  = flags added to the link command
# CFLAGSADD   = flags added to the compile command
# LIBRARIES   = libraries added to link command
# LIBDIRS     = additional link directories
# INCDIRS     = additional include directories
# ADDSRC      = additional source files
# INSTALLDATA = directories that will be installed into the openflipper data directory
#
# TRANSLATION_LANGUAGES = language codes for translation
# TRANSLATION_ADDFILES  = additional files that should be included into the translation files
#
# LICENSEMANAGER = Compile plugin with license management
#
# Plugins can implement a acg_list_filter ("listname") macro to filter out
# unneeded headers, sources and ui files from the autogenerated 
# build sources lists (see common.cmake drop_templates macro)
#

include (ACGCommon)

# get plugin name from directory name
macro (_get_plugin_name var)
  string (REGEX MATCH "Plugin-.+[/\\]?$" _dir ${CMAKE_CURRENT_SOURCE_DIR})
  string (REPLACE "Plugin-" "" ${var} ${_dir})
endmacro ()

# parse plugin macro parameter
macro (_get_plugin_parameters _prefix)
    set (_current_var _foo)
    set (_supported_var DIRS DEPS OPTDEPS LDFLAGSADD CFLAGSADD LIBRARIES LIBDIRS INCDIRS ADDSRC INSTALLDATA TRANSLATION_LANGUAGES TRANSLATION_ADDFILES)
    set (_supported_flags LICENSEMANAGER)
    foreach (_val ${_supported_var})
        set (${_prefix}_${_val})
    endforeach ()
    foreach (_val ${_supported_flags})
        set (${_prefix}_${_val} 0)
    endforeach ()

    foreach (_val ${ARGN})
        set (_found FALSE)
        foreach (_find ${_supported_var})
            if ("${_find}" STREQUAL "${_val}")
                set (_found TRUE)
            endif ()
        endforeach ()

        foreach (_find ${_supported_flags})
            if ("${_find}" STREQUAL "${_val}")
                set (${_prefix}_${_val} 1)
            endif ()
        endforeach ()

        if (_found)
            set (_current_var ${_prefix}_${_val})
        else ()
            list (APPEND ${_current_var} ${_val})
        endif ()
    endforeach ()

endmacro ()

# check dependencies
macro (_check_plugin_deps _prefix)
    set (${_prefix}_HAS_DEPS TRUE)
    foreach (_val ${ARGN})
        string (TOUPPER ${_val} _VAL)

        find_package(${_val})

        if (${_val}_FOUND OR ${_VAL}_FOUND)
          foreach (_name ${_val} ${_VAL})
            if (DEFINED ${_name}_INCLUDE_PATH)
              list (APPEND ${_prefix}_DEPS_INCDIRS "${${_name}_INCLUDE_PATH}")
            endif ()
            if (DEFINED ${_name}_INCLUDE_DIRS)
              list (APPEND ${_prefix}_DEPS_INCDIRS "${${_name}_INCLUDE_DIRS}")
            endif ()
            if (DEFINED ${_name}_INCLUDE_DIR)
              list (APPEND ${_prefix}_DEPS_INCDIRS "${${_name}_INCLUDE_DIR}")
            endif ()
            if (DEFINED ${_name}_LIBRARY_DIRS)
              list (APPEND ${_prefix}_DEPS_LIBDIRS "${${_name}_LIBRARY_DIRS}")
            endif ()
            if (DEFINED ${_name}_LIBRARY_DIR)
              list (APPEND ${_prefix}_DEPS_LIBDIRS "${${_name}_LIBRARY_DIR}")
            endif ()
            if (DEFINED ${_name}_LIBRARIES)
              list (APPEND ${_prefix}_DEPS_LIBRARIES "${${_name}_LIBRARIES}")
            endif ()
            if (DEFINED ${_name}_LIBRARY)
              list (APPEND ${_prefix}_DEPS_LIBRARIES "${${_name}_LIBRARY}")
            endif ()
            if (DEFINED ${_name}_LINKER_FLAGS)
              list (APPEND ${_prefix}_DEPS_LINKER_FLAGS "${${_name}_LINKER_FLAGS}")
            endif ()
          endforeach ()
        else ()
            set (${_prefix}_HAS_DEPS FALSE)
            acg_set (_${_prefix}_MISSING_DEPS "${_${_prefix}_MISSING_DEPS} ${_val}")
        endif ()
    endforeach ()
endmacro ()

macro (_plugin_licensemanagement)
  acg_append_files (headers "*.hh" "${CMAKE_SOURCE_DIR}/OpenFlipper/LicenseManager")
  acg_append_files (sources "*.cc" "${CMAKE_SOURCE_DIR}/OpenFlipper/LicenseManager")
#  acg_append_files (ui "*.ui" "${CMAKE_SOURCE_DIR}/OpenFlipper/LicenseManager")

  acg_append_files (keygen_hdr "*.hh" "${CMAKE_SOURCE_DIR}/OpenFlipper/LicenseManager/keyGen")
  acg_append_files (keygen_src "*.cc" "${CMAKE_SOURCE_DIR}/OpenFlipper/LicenseManager/keyGen")
  acg_append_files (keygen_ui  "*.ui" "${CMAKE_SOURCE_DIR}/OpenFlipper/LicenseManager/keyGen")
  
  # genereate uic and moc targets
  acg_qt4_autouic (keygen_uic ${keygen_ui})
  acg_qt4_automoc (keygen_moc ${keygen_hdr})

  add_executable (Plugin-${plugin}-keygen ${keygen_uic} ${keygen_moc} ${keygen_hdr} ${keygen_src})

  target_link_libraries (
    Plugin-${plugin}-keygen
    ${QT_LIBRARIES}
  )

  # create our output directroy
  if (NOT EXISTS ${CMAKE_BINARY_DIR}/LicenseManagement)
    file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/LicenseManagement)
  endif ()

  if (WIN32)
    # copy exe file to "Build" directory
    # Visual studio will create this file in a subdirectory so we can't use
    # RUNTIME_OUTPUT_DIRECTORY directly here
    add_custom_command (TARGET Plugin-${plugin}-keygen POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E
                        copy_if_different
                          ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Plugin-${plugin}-keygen.exe
                          ${CMAKE_BINARY_DIR}/LicenseManagement/Plugin-${plugin}-keygen.exe)
  else ()
    set_target_properties (
      Plugin-${plugin}-keygen PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LicenseManagement"
    )
  endif ()
endmacro ()

# main function
function (_build_openflipper_plugin plugin)

  acg_set (OPENFLIPPER_${_PLUGIN}_BUILD "0")

  # get upper plugin name
  string (TOUPPER ${plugin} _PLUGIN)

  # parse parameters
  _get_plugin_parameters (${_PLUGIN} ${ARGN})


  # check dependencies
  acg_unset (_${_PLUGIN}_MISSING_DEPS)
  set (${_PLUGIN}_HAS_DEPS)
  _check_plugin_deps (${_PLUGIN} ${${_PLUGIN}_OPTDEPS})

  acg_unset (_${_PLUGIN}_MISSING_DEPS)
  set (${_PLUGIN}_HAS_DEPS)
  _check_plugin_deps (${_PLUGIN} ${${_PLUGIN}_DEPS})

  if (${_PLUGIN}_HAS_DEPS)
    include_directories (
      .
      ${CMAKE_SOURCE_DIR}
      ${CMAKE_SOURCE_DIR}/OpenMesh/src
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_CURRENT_BINARY_DIR}
      ${${_PLUGIN}_DEPS_INCDIRS}
      ${${_PLUGIN}_INCDIRS}
      ${OPENGL_INCLUDE_DIR}
      ${GLEW_INCLUDE_DIR}
      ${GLUT_INCLUDE_DIR}
    )

    link_directories (
      ${${_PLUGIN}_DEPS_LIBDIRS}
      ${${_PLUGIN}_LIBDIRS}
    )

    set (directories 
      . 
      ${${_PLUGIN}_DIRS}
    )

    # collect all header,source and ui files
    acg_append_files (headers "*.hh" ${directories})
    acg_append_files (sources "*.cc" ${directories})
    acg_append_files (sources "*.c" ${directories})
    acg_append_files (ui "*.ui" ${directories})

    # remove template cc files from source file list
    acg_drop_templates (sources)

    # filter header,source and ui files lists if acg_list_filter macro has been defined
    if (COMMAND acg_list_filter)
      acg_list_filter (headers)
      acg_list_filter (sources)
      acg_list_filter (ui)
    endif ()

    if (${_PLUGIN}_LICENSEMANAGER)
      _plugin_licensemanagement ()
    endif ()

    # genereate uic and moc targets
    acg_qt4_autouic (uic_targets ${ui})
    acg_qt4_automoc (moc_targets ${headers})

    add_library (Plugin-${plugin} MODULE ${uic_targets} ${sources} ${headers} ${moc_targets} ${${_PLUGIN}_ADDSRC})

    # add this plugin to build plugin list for dependency tracking
    acg_set (OPENFLIPPER_PLUGINS "${OPENFLIPPER_PLUGINS};Plugin-${plugin}")
    acg_set (OPENFLIPPER_${_PLUGIN}_BUILD "1")

    # append compiler and linker flags from plugin dependencies
    set_target_properties (
      Plugin-${plugin} PROPERTIES
      COMPILE_FLAGS ""
      LINK_FLAGS "${${_PLUGIN}_LDFLAGSADD} ${${_PLUGIN}_DEPS_LINKER_FLAGS}"
    )
    
    if (WIN32)
      # Visual studio requires our plugins to link with GLUT
      find_package (GLUT)
      # generate dllinport defines
      add_definitions (-DACGDLL -DUSEACG -DPLUGINLIBDLL -DUSEPLUGINLIBDLL)
      target_link_libraries (Plugin-${plugin}
	OpenMeshCore
        OpenMeshTools
        ACG
        PluginLib
        ${GLUT_LIBRARIES}
      )

      # copy plugin dll file to "Build" directory
      # Visual studio will create this file in a subdirectory so we can't use
      # LIBRARY_OUTPUT_DIRECTORY directly here
      if (NOT EXISTS ${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_PLUGINDIR})
        file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Build/${ACG_PROJEC_PLUGINDIR})
      endif ()
      add_custom_command (TARGET Plugin-${plugin} POST_BUILD
                          COMMAND ${CMAKE_COMMAND} -E
                          copy_if_different
                            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Plugin-${plugin}.dll
                            ${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_PLUGINDIR}/Plugin-${plugin}.dll
                          )
    elseif (APPLE) 

      target_link_libraries (
         Plugin-${plugin}
	 PluginLib
	 ${QT_LIBRARIES}
      )
      # copy plugin so file to application bundle inside "Build" directory
      if (NOT EXISTS ${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_PLUGINDIR})
        file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_PLUGINDIR})
      endif ()
      add_custom_command (TARGET Plugin-${plugin} POST_BUILD
                          COMMAND ${CMAKE_COMMAND} -E
                          copy_if_different
                            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/libPlugin-${plugin}.so
                            ${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_PLUGINDIR}/libPlugin-${plugin}.so
                          )
    else ()
      # directly generate plugin in plugin directory
      set_target_properties (
        Plugin-${plugin} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_PLUGINDIR}"
      )
    endif ()
    
    target_link_libraries (Plugin-${plugin}
      ${${_PLUGIN}_DEPS_LIBRARIES}
      ${${_PLUGIN}_LIBRARIES}
    )

    # no install on mac, because the whole bundle will be installed in the
    # toplevel CMakeLists.txt
    if (NOT APPLE)
      install (
        TARGETS Plugin-${plugin}
        DESTINATION ${ACG_PROJECT_PLUGINDIR}
      )
    endif ()

    foreach (_dir ${${_PLUGIN}_INSTALLDATA})
      acg_copy_after_build (Plugin-${plugin} "${CMAKE_CURRENT_SOURCE_DIR}/${_dir}" "${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_DATADIR}/${_dir}")
      if (NOT APPLE)
        acg_install_dir ("${CMAKE_CURRENT_SOURCE_DIR}/${_dir}" "${ACG_PROJECT_DATADIR}/${_dir}")
      endif ()
    endforeach ()

    if ( ${_PLUGIN}_TRANSLATION_LANGUAGES)
      set (trans_files ${uic_targets})
      list (APPEND trans_files ${sources})
      list (APPEND trans_files ${headers})
      list (APPEND trans_files ${${_PLUGIN}_TRANSLATION_ADDFILES})
      acg_add_translation(Plugin-${plugin} ${${_PLUGIN}_TRANSLATION_LANGUAGES} ${trans_files})
    endif ()

  else ()
    message (STATUS "[WARNING] One or more dependencies for plugin ${plugin} not found. Skipping plugin.")
    message (STATUS "Missing dependencies :${_${_PLUGIN}_MISSING_DEPS}")
  endif ()
endfunction ()

macro (openflipper_plugin)
  _get_plugin_name (_plugin)

  string (TOUPPER ${_plugin} _PLUGIN)

  # add option to disable plugin build
  option (
    DISABLE_PLUGIN_${_PLUGIN}
    "Disable building of plugin \"${_plugin}\""
        OFF
  )

  if (NOT DISABLE_PLUGIN_${_PLUGIN})
    _build_openflipper_plugin (${_plugin} ${ARGN})
  endif ()
endmacro ()
