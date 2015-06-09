
#include "gl.hh"

#include <cstdlib>
#include <sstream>
#include <iostream>



//=============================================================================
namespace ACG {
//=============================================================================
namespace {
/**
 * Helper function for checkExtensionSupported(). Only for internal use.
 *
 * @return The string returned by glGetString(GL_EXTENSIONS) or an empty string
 * if glGetString() returned a null pointer.
 */
inline const char *_getExtensionString() {
    const char *supported_cstr = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
    if (supported_cstr == 0) {
        std::cerr << "\x1b[1;31mACG::checkExtensionsSupported: "
                "glGetString(GL_EXTENSIONS) call failed.\x1b[0m\n";
        return "";
    }
    return supported_cstr;
}
}

/** Check if the extension given by a std::string is supported by the current OpenGL extension
*/
bool checkExtensionSupported( const std::string& _extension )  {
    /**
     * Cache glGetString(...) output because depending on the graphics driver
     * it might return 0 the second time we call it.
     */
    static const std::string supported_str(_getExtensionString());

    /*
     * supported_str is a space delimited list. Tokenize it. Simply searching
     * for _extension within supported_str yields false positives if the
     * requested extension is a substring of a supported one.
     */
    for (std::istringstream supported(supported_str); !supported.eof(); ) {
        std::string feature;
        supported >> feature;
        if (feature == _extension) return true;
    }
    return false;
}

/** Check if OpenGL Version is greater or equal than the given values
*/
bool openGLVersion( const int _major, const int _minor  )  {

  // Read OpenGL Version string
  std::string glVersionString = (const char*)glGetString(GL_VERSION);

  // Use string stream to parse
  std::istringstream stream;
  stream.str(glVersionString);

  // Buffer for the dot between major and minor
  char dot;

  // Read Major version number
  int major ;
  stream >> major;
  stream >> dot;

  // Read minor version number
  int minor;
  stream >> minor;

  if ( (_major > major) || ( (_major == major) && (_minor > minor))  ) {
    std::cerr << "OpenGL Version check failed. Required  : " << _major << "." << _minor << std::endl;
    std::cerr << "OpenGL Version check failed. Available : " << major << "." << minor << std::endl;
    return false;
  }

  return true;
}


//=============================================================================
}  // namespace ACG
//=============================================================================
