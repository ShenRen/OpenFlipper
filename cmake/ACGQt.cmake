#unset cached qt variables which are set by all qt versions. version is the major number of the qt version (e.g. 4 or 5, not 4.8)
macro (acg_unset_qt_shared_variables version)
  if (ACG_INTERNAL_QT_LAST_VERSION)
    if (NOT ${ACG_INTERNAL_QT_LAST_VERSION} EQUAL ${version})
      unset(QT_BINARY_DIR)
      unset(QT_PLUGINS_DIR)
      unset(ACG_INTERNAL_QT_LAST_VERSION)
    endif()
  endif()
  set (ACG_INTERNAL_QT_LAST_VERSION "${version}" CACHE INTERNAL "Qt Version, which was used on the last time")
endmacro()

# look for selected qt dependencies
macro (acg_qt4)
  if (NOT QT4_FOUND)
    acg_unset_qt_shared_variables(4)
    set (QT_MIN_VERSION ${ARGN}) 

    if(POLICY CMP0020)
      # Automatically link Qt executables to qtmain target on Windows
      cmake_policy(SET CMP0020 NEW)
    endif(POLICY CMP0020)

    find_package (Qt4 COMPONENTS QtCore QtGui )

    set (QT_USE_QTOPENGL 1)
    set (QT_USE_QTNETWORK 1)
    set (QT_USE_QTSCRIPT 1)
    set (QT_USE_QTSQL 1)
    set (QT_USE_QTXML 1)
    set (QT_USE_QTXMLPATTERNS 1)
    set (QT_USE_QTHELP 1)
    set (QT_USE_QTWEBKIT 1)
    set (QT_USE_QTUITOOLS 1)

    if (QT_QTSCRIPTTOOLS_FOUND)
      set (QT_USE_QTSCRIPTTOOLS 1)
    endif()

    include (${QT_USE_FILE})
  endif ()
endmacro ()

macro (acg_qt5)

   if(POLICY CMP0020)
     # Automatically link Qt executables to qtmain target on Windows
     cmake_policy(SET CMP0020 NEW)
   endif(POLICY CMP0020)
  #if (NOT QT5_FOUND)

    #set (QT_MIN_VERSION ${ARGN})

  #try to find qt5 automatically
  #for custom installation of qt5, dont use any of these variables
  set (QT5_INSTALL_PATH "" CACHE PATH "Path to Qt5 directory which contains lib and include folder")
  if (EXISTS "${QT5_INSTALL_PATH}")
    set (CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${QT5_INSTALL_PATH}")
    set (QT5_INSTALL_PATH_EXISTS TRUE)
  endif()
  
  
    
    find_package (Qt5Core QUIET)
    if(Qt5Core_FOUND)
        string(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" QT_VERSION_MAJOR "${Qt5Core_VERSION_STRING}")
        string(REGEX REPLACE "^[0-9]+\\.([0-9])+\\.[0-9]+.*" "\\1" QT_VERSION_MINOR "${Qt5Core_VERSION_STRING}")
        string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" QT_VERSION_PATCH "${Qt5Core_VERSION_STRING}")

      #find WINDOWS_SDK to avoid qt error. This must be done BEFORE Qt5Widgets is searched
      if (WIN32)
        if ( (QT_VERSION_MAJOR EQUAL 5) AND (QT_VERSION_MINOR LESS 3 OR ( QT_VERSION_MINOR EQUAL 3 AND QT_VERSION_PATCH EQUAL 0 )) ) # for all Qt version > 5.0.0 and < 5.3.1
          #glu32.lib is needed by qt5 opengl version. it cannot find it by itself so we help qt
          #this block has to be executed, before Qt5Gui is searched, otherwise we will end up with the (not so useful) QT5 error message
          set(WINDOWS_SDK_LIBS "COULD_NOT_FOUND" CACHE PATH "Path to the latest windows sdk libs which includes glu32.lib. Used by Qt5.")
          if (EXISTS "${WINDOWS_SDK_LIBS}\\glu32.lib")
            set (CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${WINDOWS_SDK_LIBS}")
          elseif(QT5_INSTALL_PATH_EXISTS) #trying to install qt5. notify about missing sdk before the qt message comes
            message(FATAL_ERROR "Could not find glu32.lib. This is necessary for QT5 OpenGL version for windows, spleace specify glu32.lib in WINDOWS_SDK_LIB or install Qt version >= 5.3.1")
          endif()
        endif()
      endif(WIN32)

      #do noot look for Webkit and qt declarative on qt version 5.6 or newer
      if(${QT_VERSION_MINOR} GREATER 5)
          find_package (Qt5Widgets QUIET)
          find_package (Qt5Gui QUIET)
          find_package (Qt5OpenGL QUIET)
          find_package (Qt5Network QUIET)
          find_package (Qt5Script QUIET)
          find_package (Qt5ScriptTools QUIET)
          find_package (Qt5Sql QUIET)
          find_package (Qt5Xml QUIET)
          find_package (Qt5XmlPatterns QUIET)
          find_package (Qt5Help QUIET)
          find_package (Qt5UiTools QUIET)
          find_package (Qt5Concurrent QUIET)
          find_package (Qt5PrintSupport QUIET)
          find_package (Qt5Svg QUIET)
      else(${QT_VERSION_MINOR} GREATER 5)
          find_package (Qt5Declarative QUIET)
          find_package (Qt5Widgets QUIET)
          find_package (Qt5Gui QUIET)
          find_package (Qt5OpenGL QUIET)
          find_package (Qt5Network QUIET)
          find_package (Qt5Script QUIET)
          find_package (Qt5ScriptTools QUIET)
          find_package (Qt5Sql QUIET)
          find_package (Qt5Xml QUIET)
          find_package (Qt5XmlPatterns QUIET)
          find_package (Qt5Help QUIET)
          find_package (Qt5WebKit QUIET)
          find_package (Qt5UiTools QUIET)
          find_package (Qt5Concurrent QUIET)
          find_package (Qt5PrintSupport QUIET)
          find_package (Qt5Svg QUIET)
      endif(${QT_VERSION_MINOR} GREATER 5)

      if (NOT WIN32 AND NOT APPLE)
         find_package (Qt5X11Extras QUIET)
      endif ()

      if(${QT_VERSION_MINOR} GREATER 5)
          if (Qt5Core_FOUND AND Qt5Widgets_FOUND
            AND Qt5Gui_FOUND AND Qt5OpenGL_FOUND AND Qt5Network_FOUND
            AND Qt5Script_FOUND AND Qt5ScriptTools_FOUND AND Qt5Sql_FOUND
            AND Qt5Xml_FOUND AND Qt5XmlPatterns_FOUND AND Qt5Help_FOUND
            AND Qt5UiTools_FOUND AND Qt5Concurrent_FOUND
            AND Qt5PrintSupport_FOUND)
            set (QT5_FOUND TRUE)
          endif()
      else(${QT_VERSION_MINOR} GREATER 5)
          if (Qt5Core_FOUND AND Qt5Declarative_FOUND AND Qt5Widgets_FOUND
            AND Qt5Gui_FOUND AND Qt5OpenGL_FOUND AND Qt5Network_FOUND
            AND Qt5Script_FOUND AND Qt5ScriptTools_FOUND AND Qt5Sql_FOUND
            AND Qt5Xml_FOUND AND Qt5XmlPatterns_FOUND AND Qt5Help_FOUND
            AND Qt5WebKit_FOUND AND Qt5UiTools_FOUND AND Qt5Concurrent_FOUND
            AND Qt5PrintSupport_FOUND)
            set (QT5_FOUND TRUE)
          endif()
      endif(${QT_VERSION_MINOR} GREATER 5)
    endif(Qt5Core_FOUND)
    
    if (QT5_FOUND)   
      acg_unset_qt_shared_variables(5)
    
      #set plugin dir
      list(GET Qt5Gui_PLUGINS 0 _plugin)
      if (_plugin)
        get_target_property(_plugin_full ${_plugin} LOCATION)
        get_filename_component(_plugin_dir ${_plugin_full} PATH)
      set (QT_PLUGINS_DIR "${_plugin_dir}/../" CACHE PATH "Path to the qt plugin directory")
      elseif(QT5_INSTALL_PATH_EXISTS)
        set (QT_PLUGINS_DIR "${QT5_INSTALL_PATH}/plugins/" CACHE PATH "Path to the qt plugin directory")
      elseif()
        set (QT_PLUGINS_DIR "QT_PLUGIN_DIR_NOT_FOUND" CACHE PATH "Path to the qt plugin directory")
      endif(_plugin)
      
      #set binary dir for fixupbundle
      if(QT5_INSTALL_PATH_EXISTS)
        set(_QT_BINARY_DIR "${QT5_INSTALL_PATH}/bin")
      else()
        get_target_property(_QT_BINARY_DIR ${Qt5Widgets_UIC_EXECUTABLE} LOCATION)
        get_filename_component(_QT_BINARY_DIR ${_QT_BINARY_DIR} PATH)
      endif(QT5_INSTALL_PATH_EXISTS)
      
      set (QT_BINARY_DIR "${_QT_BINARY_DIR}" CACHE PATH "Qt5 binary Directory")
      mark_as_advanced(QT_BINARY_DIR)
      
      set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  
      include_directories(${Qt5Core_INCLUDE_DIRS})
      include_directories(${Qt5Declarative_INCLUDE_DIRS})
      include_directories(${Qt5Widgets_INCLUDE_DIRS})
      include_directories(${Qt5Gui_INCLUDE_DIRS})
      include_directories(${Qt5OpenGL_INCLUDE_DIRS})
      include_directories(${Qt5Network_INCLUDE_DIRS})
      include_directories(${Qt5Script_INCLUDE_DIRS})
      include_directories(${Qt5ScriptTools_INCLUDE_DIRS})
      include_directories(${Qt5Sql_INCLUDE_DIRS})
      include_directories(${Qt5Xml_INCLUDE_DIRS})
      include_directories(${Qt5XmlPatterns_INCLUDE_DIRS})
      include_directories(${Qt5Help_INCLUDE_DIRS})
      include_directories(${Qt5WebKit_INCLUDE_DIRS})
      include_directories(${Qt5UiTools_INCLUDE_DIRS})
      include_directories(${Qt5Concurrent_INCLUDE_DIRS})
      include_directories(${Qt5PrintSupport_INCLUDE_DIRS})
      add_definitions(${Qt5Core_DEFINITIONS})
      add_definitions(${Qt5Widgets_DEFINITIONS})
      add_definitions(${Qt5Gui_DEFINITIONS})
      add_definitions(${Qt5OpenGL_DEFINITIONS})
      add_definitions(${Qt5Network_DEFINITIONS})
      add_definitions(${Qt5Script_DEFINITIONS})
      add_definitions(${Qt5ScriptTools_DEFINITIONS})
      add_definitions(${Qt5Sql_DEFINITIONS})
      add_definitions(${Qt5Xml_DEFINITIONS})
      add_definitions(${Qt5XmlPatterns_DEFINITIONS})
      add_definitions(${Qt5Help_DEFINITIONS})
      add_definitions(${Qt5WebKit_DEFINITIONS})
      add_definitions(${Qt5UiTools_DEFINITIONS})
      add_definitions(${Qt5Concurrent_DEFINITIONS})
      add_definitions(${Qt5PrintSupport_DEFINITIONS})
      
      if (Qt5X11Extras_FOUND)
	      include_directories(${Qt5X11Extras_INCLUDE_DIRS})
	      add_definitions(${Qt5X11Extras_DEFINITIONS})
      endif ()
      
      if ( NOT MSVC )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
      endif()

      set (QT_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Declarative_LIBRARIES} ${Qt5Widgets_LIBRARIES}
        ${Qt5Gui_LIBRARIES} ${Qt5OpenGL_LIBRARIES} ${Qt5Network_LIBRARIES}
        ${Qt5Script_LIBRARIES} ${Qt5ScriptTools_LIBRARIES} ${Qt5Sql_LIBRARIES}
        ${Qt5Xml_LIBRARIES} ${Qt5XmlPatterns_LIBRARIES} ${Qt5Help_LIBRARIES}
        ${Qt5WebKit_LIBRARIES} ${Qt5UiTools_LIBRARIES} ${Qt5Concurrent_LIBARIES} 
        ${Qt5PrintSupport_LIBRARIES})
        
      if (Qt5X11Extras_FOUND)
	      list (APPEND QT_LIBRARIES ${Qt5X11Extras_LIBRARIES})
	  endif ()
       
      if (MSVC)
        set (QT_LIBRARIES ${QT_LIBRARIES} ${Qt5Core_QTMAIN_LIBRARIES})
      endif()

	  #add_definitions(-DQT_NO_OPENGL)

	  #adding QT_NO_DEBUG to all release modes. 
	  #  Note: for multi generators like msvc you cannot set this definition depending of
	  #  the current build type, because it may change in the future inside the ide and not via cmake
	  if (MSVC_IDE)
	    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /DQT_NO_DEBUG")
	    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /DQT_NO_DEBUG")
		
		set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_RELEASE} /DQT_NO_DEBUG")
	    set(CMAKE_CXX_FLAGS_MINSITEREL "${CMAKE_C_FLAGS_RELEASE} /DQT_NO_DEBUG")
		
		set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE} /DQT_NO_DEBUG")
	    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE} /DQT_NO_DEBUG")
	  else(MSVC_IDE)
	    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	      add_definitions(-DQT_NO_DEBUG)
	    endif()
      endif(MSVC_IDE)

    endif ()
endmacro ()

# generate moc targets for sources in list
macro (acg_qt4_automoc moc_SRCS)
  qt4_get_moc_flags (_moc_INCS)
  
  list(REMOVE_DUPLICATES _moc_INCS)

  set (_matching_FILES )
  foreach (_current_FILE ${ARGN})

     get_filename_component (_abs_FILE ${_current_FILE} ABSOLUTE)
     # if "SKIP_AUTOMOC" is set to true, we will not handle this file here.
     # here. this is required to make bouic work correctly:
     # we need to add generated .cpp files to the sources (to compile them),
     # but we cannot let automoc handle them, as the .cpp files don't exist yet when
     # cmake is run for the very first time on them -> however the .cpp files might
     # exist at a later run. at that time we need to skip them, so that we don't add two
     # different rules for the same moc file
     get_source_file_property (_skip ${_abs_FILE} SKIP_AUTOMOC)

     if ( NOT _skip AND EXISTS ${_abs_FILE} )

        file (READ ${_abs_FILE} _contents)

        get_filename_component (_abs_PATH ${_abs_FILE} PATH)

        string (REGEX MATCHALL "Q_OBJECT" _match "${_contents}")
        if (_match)
            get_filename_component (_basename ${_current_FILE} NAME_WE)
            set (_header ${_abs_FILE})
            set (_moc    ${CMAKE_CURRENT_BINARY_DIR}/moc_${_basename}.cpp)

            add_custom_command (OUTPUT ${_moc}
                COMMAND ${QT_MOC_EXECUTABLE}
                ARGS ${_moc_INCS} ${_header} -o ${_moc}
                DEPENDS ${_header}
            )

            add_file_dependencies (${_abs_FILE} ${_moc})
            set (${moc_SRCS} ${${moc_SRCS}} ${_moc})
            
        endif ()
     endif ()
  endforeach ()
endmacro ()

# generate moc targets for sources in list
macro (acg_qt5_automoc moc_SRCS)
  qt5_get_moc_flags (_moc_INCS)
  
  list(REMOVE_DUPLICATES _moc_INCS)

  set (_matching_FILES )
  foreach (_current_FILE ${ARGN})

     get_filename_component (_abs_FILE ${_current_FILE} ABSOLUTE)
     # if "SKIP_AUTOMOC" is set to true, we will not handle this file here.
     # here. this is required to make bouic work correctly:
     # we need to add generated .cpp files to the sources (to compile them),
     # but we cannot let automoc handle them, as the .cpp files don't exist yet when
     # cmake is run for the very first time on them -> however the .cpp files might
     # exist at a later run. at that time we need to skip them, so that we don't add two
     # different rules for the same moc file
     get_source_file_property (_skip ${_abs_FILE} SKIP_AUTOMOC)

     if ( NOT _skip AND EXISTS ${_abs_FILE} )

        file (READ ${_abs_FILE} _contents)

        get_filename_component (_abs_PATH ${_abs_FILE} PATH)

        string (REGEX MATCHALL "Q_OBJECT" _match "${_contents}")
        if (_match)
            get_filename_component (_basename ${_current_FILE} NAME_WE)
            set (_header ${_abs_FILE})
            set (_moc    ${CMAKE_CURRENT_BINARY_DIR}/moc_${_basename}.cpp)

            add_custom_command (OUTPUT ${_moc}
                COMMAND ${QT_MOC_EXECUTABLE}
                ARGS ${_moc_INCS} ${_header} -o ${_moc}
                DEPENDS ${_header}
            )

            add_file_dependencies (${_abs_FILE} ${_moc})
            set (${moc_SRCS} ${${moc_SRCS}} ${_moc})

        endif ()
     endif ()
  endforeach ()
endmacro ()

# generate uic targets for sources in list
macro (acg_qt4_autouic uic_SRCS)

  set (_matching_FILES )
  foreach (_current_FILE ${ARGN})

     get_filename_component (_abs_FILE ${_current_FILE} ABSOLUTE)

     if ( EXISTS ${_abs_FILE} )

        file (READ ${_abs_FILE} _contents)

        get_filename_component (_abs_PATH ${_abs_FILE} PATH)

        get_filename_component (_basename ${_current_FILE} NAME_WE)
        string (REGEX REPLACE "Ui$" "" _cbasename ${_basename})
        set (_outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${_basename}.hh)
        set (_header ${_basename}.hh)
        set (_source ${_abs_PATH}/${_cbasename}.cc)
        
        add_custom_command (OUTPUT ${_outfile}
            COMMAND ${QT_UIC_EXECUTABLE}
            ARGS -o ${_outfile} ${_abs_FILE}
            MAIN_DEPENDENCY ${_abs_FILE} VERBATIM)

        add_file_dependencies (${_source} ${_outfile})
        set (${uic_SRCS} ${${uic_SRCS}} ${_outfile})
            
     endif ()
  endforeach ()
endmacro ()

# generate uic targets for sources in list
macro (acg_qt5_autouic uic_SRCS)

  set (_matching_FILES )
  foreach (_current_FILE ${ARGN})

     get_filename_component (_abs_FILE ${_current_FILE} ABSOLUTE)

     if ( EXISTS ${_abs_FILE} )

        file (READ ${_abs_FILE} _contents)

        get_filename_component (_abs_PATH ${_abs_FILE} PATH)

        get_filename_component (_basename ${_current_FILE} NAME_WE)
        string (REGEX REPLACE "Ui$" "" _cbasename ${_basename})
        set (_outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${_basename}.hh)
        set (_header ${_basename}.hh)
        set (_source ${_abs_PATH}/${_cbasename}.cc)

        add_custom_command (OUTPUT ${_outfile}
            COMMAND ${Qt5Widgets_UIC_EXECUTABLE}
            ARGS -o ${_outfile} ${_abs_FILE}
            MAIN_DEPENDENCY ${_abs_FILE} VERBATIM)

        add_file_dependencies (${_source} ${_outfile})
        set (${uic_SRCS} ${${uic_SRCS}} ${_outfile})

     endif ()
  endforeach ()
endmacro ()

# generate qrc targets for sources in list
macro (acg_qt4_autoqrc qrc_SRCS)

  set (_matching_FILES )
  foreach (_current_FILE ${ARGN})

     get_filename_component (_abs_FILE ${_current_FILE} ABSOLUTE)

     if ( EXISTS ${_abs_FILE} )

        file (READ ${_abs_FILE} _contents)

        get_filename_component (_abs_PATH ${_abs_FILE} PATH)

        get_filename_component (_basename ${_current_FILE} NAME_WE)
        set (_outfile ${CMAKE_CURRENT_BINARY_DIR}/qrc_${_basename}.cpp)
        
        add_custom_command (OUTPUT ${_outfile}
            COMMAND ${QT_RCC_EXECUTABLE}
            ARGS -o ${_outfile}  ${_abs_FILE}
            DEPENDS ${_abs_FILE}) 

        add_file_dependencies (${_source} ${_outfile})
        set (${qrc_SRCS} ${${qrc_SRCS}} ${_outfile})

     endif ()
  endforeach ()
endmacro ()

# generate qrc targets for sources in list
macro (acg_qt5_autoqrc qrc_SRCS)

  set (_matching_FILES )
  foreach (_current_FILE ${ARGN})

     get_filename_component (_abs_FILE ${_current_FILE} ABSOLUTE)

     if ( EXISTS ${_abs_FILE} )

        file (READ ${_abs_FILE} _contents)

        get_filename_component (_abs_PATH ${_abs_FILE} PATH)

        get_filename_component (_basename ${_current_FILE} NAME_WE)
        set (_outfile ${CMAKE_CURRENT_BINARY_DIR}/qrc_${_basename}.cpp)

        qt5_add_resources( ${_outfile}  ${_abs_FILE}  )

        add_file_dependencies (${_source} ${_outfile})
        set (${qrc_SRCS} ${${qrc_SRCS}} ${_outfile})

     endif ()
  endforeach ()
endmacro ()


#generates qt translations
function (acg_add_translations _target _languages _sources)

  string (TOUPPER ${_target} _TARGET)
  # generate/use translation files
  # run with UPDATE_TRANSLATIONS set to on to build qm files
  option (UPDATE_TRANSLATIONS_${_TARGET} "Update source translation *.ts files (WARNING: make clean will delete the source .ts files! Danger!)")

  set (_new_ts_files)
  set (_ts_files)

  foreach (lang ${_languages})
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/translations/${_target}_${lang}.ts" OR UPDATE_TRANSLATIONS_${_TARGET})
      list (APPEND _new_ts_files "translations/${_target}_${lang}.ts")
    else ()
      list (APPEND _ts_files "translations/${_target}_${lang}.ts")
    endif ()
  endforeach ()


  set (_qm_files)
  if ( _new_ts_files )
    if (QT5_FOUND)
      #qt5_create_translation(_qm_files ${_sources} ${_new_ts_files})
    elseif (QT4_FOUND)
    qt4_create_translation(_qm_files ${_sources} ${_new_ts_files})
  endif ()
  endif ()

  if ( _ts_files )
    if (QT5_FOUND)
      #qt5_add_translation(_qm_files2 ${_ts_files})
    elseif (QT4_FOUND)
    qt4_add_translation(_qm_files2 ${_ts_files})
    endif()
    list (APPEND _qm_files ${_qm_files2})
  endif ()

  # create a target for the translation files ( and object files )
  # Use this target, to update only the translations
  add_custom_target (translations_target_${_target} DEPENDS ${_qm_files})
  GROUP_PROJECT( translations_target_${_target} "Translations")

  # Build translations with the application
  add_dependencies(${_target} translations_target_${_target} )

  if (NOT EXISTS ${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_DATADIR}/Translations)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_DATADIR}/Translations )
  endif ()

  foreach (_qm ${_qm_files})
    get_filename_component (_qm_name "${_qm}" NAME)
    add_custom_command (TARGET translations_target_${_target} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E
                        copy_if_different
                          ${_qm}
                          ${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_DATADIR}/Translations/${_qm_name})
  endforeach ()

  if (NOT ACG_PROJECT_MACOS_BUNDLE OR NOT APPLE)
    install (FILES ${_qm_files} DESTINATION "${ACG_PROJECT_DATADIR}/Translations")
  endif ()
endfunction ()

# Function that writes all generated qch files into one Help.qhcp project file
function (generate_qhp_file files_loc plugin_name)

    set(qhp_file "${files_loc}/${plugin_name}.qhp")
    # Read in template file
    file(STRINGS "${CMAKE_SOURCE_DIR}/OpenFlipper/Documentation/QtHelpResources/QtHelpProject.qhp" qhp_template)
    
    # Initialize new project file
    file(WRITE ${qhp_file} "")
    foreach (_line ${qhp_template})
        string(STRIP ${_line} stripped)
        if("${stripped}" STREQUAL "files")
            acg_get_files_in_dir (_files ${files_loc})
            foreach (_file ${_files})
                string(REGEX MATCH ".+[.]+((html)|(htm)|(xml))$" fileresult ${_file})
                string(LENGTH "${fileresult}" len)
                if(${len} GREATER 0)
                    file(APPEND ${qhp_file} "<file>${_file}</file>\n")
                endif()
            endforeach()
        else()
            string(REGEX REPLACE "plugin" ${plugin} newline ${_line})
            file(APPEND ${qhp_file} "${newline}\n")
        endif()
    endforeach()
endfunction()

