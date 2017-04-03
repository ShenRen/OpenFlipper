/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
 *           Copyright (c) 2001-2015, RWTH-Aachen University                 *
 *           Department of Computer Graphics and Multimedia                  *
 *                          All rights reserved.                             *
 *                            www.openflipper.org                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * This file is part of OpenFlipper.                                         *
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Redistribution and use in source and binary forms, with or without        *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 *                                                                           *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 *                                                                           *
 * 3. Neither the name of the copyright holder nor the names of its          *
 *    contributors may be used to endorse or promote products derived from   *
 *    this software without specific prior written permission.               *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED *
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        *
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      *
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              *
*                                                                            *
\*===========================================================================*/

/*===========================================================================*\
*                                                                            *
*   $Revision$                                                       *
*   $LastChangedBy$                                                *
*   $Date$                     *
*                                                                            *
\*===========================================================================*/



// Mainwindow

#include "OpenFlipper/Core/Core.hh"
#include "common/glew_wrappers.hh"

// Qt
#include <qgl.h>

// stdc++
#include <csignal>
#include <regex>

#include <QCommandLineParser>

#if ( defined(WIN32))
  #define NO_EXECINFO
#endif

#ifndef NO_EXECINFO
#include <execinfo.h>
#endif

#ifdef USE_OPENMP
#endif

/* ==========================================================
 *
 * Stackwalker code. Used to get a backtrace if OpenFlipper
 * crashes under windows
 *
 * ==========================================================*/


#ifdef WIN32
  #include "StackWalker/StackWalker.hh"
  #include <fstream>

  class StackWalkerToConsole : public StackWalker
  {
  protected:
	  virtual void OnOutput(LPCSTR szText)
    	{
			// Writes crash dump to .OpenFlipper config directory
			std::ofstream crashFile;
			QString crashName = OpenFlipper::Options::configDirStr() + QDir::separator() + "CrashDump.txt";
			crashFile.open(crashName.toLatin1(),std::ios::out | std::ios::app);
			crashFile << szText;
			crashFile.close();
	  }
  };
#endif


/* ==========================================================
 *
 * Console for Windows to get additional output written via
 * cerr, cout, ... that is not forwarded to log window
 *
 * ==========================================================*/

#ifdef WIN32

  void attachConsole()
   {
     //try to attach the console of the parent process
     if (AttachConsole(-1))
     {
       //if the console was attached change stdinput and output
       FILE* check = freopen("CONIN$", "r", stdin);
       if (check) {
         std::cerr << "Error reopening stdin" << std::endl;
       }
       check = freopen("CONOUT$", "w", stdout);
       if (check) {
         std::cerr << "Error reopening stdout" << std::endl;
       }
       check = freopen("CONOUT$", "w", stderr);
       if (check) {
         std::cerr << "Error reopening stderr" << std::endl;
       }
     }
     else
     {
       //create and attach a new console if needed
 #ifndef NDEBUG
       //always open a console in debug mode
       AllocConsole();

       FILE* check = freopen("CONIN$", "r", stdin);
       if (check) {
         std::cerr << "Error reopening stdin" << std::endl;
       }
       check = freopen("CONOUT$", "w", stdout);
       if (check) {
         std::cerr << "Error reopening stdout" << std::endl;
       }
       check = freopen("CONOUT$", "w", stderr);
       if (check) {
         std::cerr << "Error reopening stderr" << std::endl;
       }
       return;
 #endif
       if (OpenFlipper::Options::logToConsole())
       {
         AllocConsole();

         FILE* check = freopen("CONIN$", "r", stdin);
         if (check) {
           std::cerr << "Error reopening stdin" << std::endl;
         }
         check = freopen("CONOUT$", "w", stdout);
         if (check) {
           std::cerr << "Error reopening stdout" << std::endl;
         }
         check = freopen("CONOUT$", "w", stderr);
         if (check) {
           std::cerr << "Error reopening stderr" << std::endl;
         }
       }
     }
   }

#endif

/* ==========================================================
 *
 * Linux function printing a full stack trace to the console
 *
 * ==========================================================*/
#ifndef NO_EXECINFO

#if defined(__GLIBCXX__) || defined(__GLIBCPP__)
// GCC: implement demangling using cxxabi
#include <cxxabi.h>
std::string demangle(const std::string& _symbol)
{
    int status;
    char* demangled = abi::__cxa_demangle(_symbol.c_str(), nullptr, nullptr, &status);
    if (demangled) {
        std::string result{demangled};
        free(demangled);
        if (status == 0) {
            return result;
        }
        else {
            return _symbol;
        }
    }
    else {
        return _symbol;
    }
}
#else
// other compiler environment: no demangling
std::string demangle(const std::string& _symbol)
{
    return _symbol;
}
#endif

void backtrace()
{
    void *addresses[20];
    char **strings;

    int size = backtrace(addresses, 20);
    strings = backtrace_symbols(addresses, size);
    std::cerr << "Stack frames: " << size << std::endl;
    // line format:
    // <path>(<mangled_name>+<offset>) [<address>]
    std::regex line_format{R"(^\s*(.+)\((([^()]+)?\+(0x[0-9a-f]+))?\)\s+\[(0x[0-9a-f]+)\]\s*$)"};
    for(int i = 0; i < size; i++) {
        std::string line{strings[i]};
        std::smatch match;
        std::regex_match(line, match, line_format);
        if (!match.empty()) {
            auto file_name = match[1].str();
            auto symbol = demangle(match[3].str());
            auto offset = match[4].str();
            auto address = match[5].str();
            std::cerr << i << ":";
            if (!file_name.empty()) std::cerr << " " << file_name << " ::";
            if (!symbol.empty()) std::cerr << " " << symbol;
            if (!offset.empty()) std::cerr << " (+" << offset << ")";
            if (!address.empty()) std::cerr << " [" << address << "]";
            std::cerr << std::endl;
        }
    }
    free(strings);
}
#endif

/* ==========================================================
 *
 * General segfault handler. This function is called if OpenFlipper
 * crashes
 *
 * ==========================================================*/
void segfaultHandling (int) {

  // prevent infinite recursion if segfaultHandling() causes another segfault
  std::signal(SIGSEGV, SIG_DFL);


  std::cerr << "\n" << std::endl;
  std::cerr << "\n" << std::endl;
  std::cerr << "\33[31m" << "=====================================================" << std::endl;
  std::cerr << "\33[31m" << "OpenFlipper or one of its plugins caused a Segfault." << std::endl;
  std::cerr << "\33[31m" << "This should not happen,... Sorry :-(" << std::endl;
  std::cerr << "\33[31m" << "=====================================================" << std::endl;
  std::cerr << "\n" << std::endl;

  // Linux Handler
#ifndef NO_EXECINFO
  std::cerr << "\33[0m"  << "Trying a backtrace to show what happened last: " << std::endl;
  backtrace();

  std::cerr << "\n" << std::endl;
  std::cerr << "Backtrace completed, trying to abort now ..." << std::endl;
#endif

  // Windows handler via StackWalker
#ifdef WIN32
  StackWalkerToConsole sw;
  sw.ShowCallstack();
#endif


  std::cerr << "Trying to get additional information (This might fail if the memory is corrupted)." << std::endl;

  if (OpenFlipper::Options::gui()) {
    for ( unsigned int i = 0 ; i <  4 ; ++i) {
      std::cerr << "DrawMode Viewer "<<  i << " " << PluginFunctions::drawMode(i).description() << std::endl;
    }
  }

  std::abort();
}

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};


bool openPolyMeshes = false;
bool remoteControl  = false;

// Parse all options
CommandLineParseResult parseCommandLine(QCommandLineParser &parser, QString *errorMessage) {

  #ifndef WIN32
  #ifndef __APPLE__
    //workaround for bug with stereo mode on Qt5.7.0 and Qt5.7.1 on Linux
    int QtVersionMajor, QtVersionMinor, QtVersionPatch;
    if(sscanf(qVersion(),"%1d.%1d.%1d",&QtVersionMajor, &QtVersionMinor, &QtVersionPatch) == 3)
    {
      if(QtVersionMajor == 5 && QtVersionMinor >= 7)
      {
        if(QtVersionPatch < 2)
        {
          std::cerr << "The used Qt Version does not support stereo mode. Disabling stereo mode." << std::endl;
          OpenFlipper::Options::stereo(false);
        }
        else
          std::cerr << "Stereo Mode has not been tested for the used Qt Version." << std::endl;
      }
    }
  #endif
  #endif


 parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);


 QCommandLineOption debugOption(QStringList() << "d" << "debug",QCoreApplication::translate("main", "Enable debugging mode"));
 parser.addOption(debugOption);

 QCommandLineOption stereoOption("disable-stereo",QCoreApplication::translate("main", "Disable stereo mode"));
 parser.addOption(stereoOption);

 QCommandLineOption batchOption(QStringList() << "b" << "batch",QCoreApplication::translate("main", "Batch mode, you have to provide a script for execution"));
 parser.addOption(batchOption);

 QCommandLineOption logConsoleOption(QStringList() << "c" << "log-to-console",QCoreApplication::translate("main", "Write logger window contents to console"));
 parser.addOption(logConsoleOption);

 QCommandLineOption remoteControlOption("remote-control",QCoreApplication::translate("main", "Batch mode accepting remote connections"));
 parser.addOption(remoteControlOption);

 QCommandLineOption fulscreenOption(QStringList() << "f" << "fullscreen",QCoreApplication::translate("main", "Start in fullscreen mode"));
 parser.addOption(fulscreenOption);

 QCommandLineOption hideLoggerOption(QStringList() << "l" << "hide-logger",QCoreApplication::translate("main", "Start with hidden log window"));
 parser.addOption(hideLoggerOption);

 QCommandLineOption hideToolboxOption(QStringList() << "t" << "hide-toolbox",QCoreApplication::translate("main", "Start with hidden toolbox"));
 parser.addOption(hideToolboxOption);

 QCommandLineOption noSplashOption("no-splash",QCoreApplication::translate("main", "Hide splash screen"));
 parser.addOption(noSplashOption);

 QCommandLineOption polyMeshOption("p",QCoreApplication::translate("main", "Open files as PolyMeshes"));
 parser.addOption(polyMeshOption);

 QCommandLineOption remotePortOption("remote-port",QCoreApplication::translate("main", "Remote port"),"portnumber");
 parser.addOption(remotePortOption);

 const QCommandLineOption helpOption = parser.addHelpOption();
 const QCommandLineOption versionOption = parser.addVersionOption();


 // Now parse the command line
 if (!parser.parse(QCoreApplication::arguments())) {
   *errorMessage = parser.errorText();
   return CommandLineError;
 }

 if (parser.isSet(helpOption))
   return CommandLineHelpRequested;

 if (parser.isSet(versionOption))
   return CommandLineVersionRequested;

 if (parser.isSet(debugOption)) {
   OpenFlipper::Options::debug(true);
 }

 if (parser.isSet(stereoOption)) {
   OpenFlipper::Options::stereo(false);
 }

 if (parser.isSet(batchOption)) {
   OpenFlipper::Options::nogui(true);
 }

 if (parser.isSet(logConsoleOption)) {
   OpenFlipper::Options::logToConsole(true);
 }

 if (parser.isSet(remoteControlOption)) {
   OpenFlipper::Options::remoteControl(true);
 }

 if (parser.isSet(fulscreenOption)) {
   OpenFlipperSettings().setValue("Core/Gui/fullscreen",true);
 }

 if (parser.isSet(hideLoggerOption)) {
   OpenFlipper::Options::loggerState(OpenFlipper::Options::Hidden);
 }

 if (parser.isSet(hideToolboxOption)) {
   OpenFlipperSettings().setValue("Core/Gui/ToolBoxes/hidden",true);
 }

 if (parser.isSet(noSplashOption)) {
   OpenFlipperSettings().setValue("Core/Gui/splash",false);
 }

 if (parser.isSet(polyMeshOption)) {
   openPolyMeshes = true;
 }

 if (parser.isSet(remotePortOption)) {
   const QString port = parser.value("remote-port");
   std::cerr << "Got port option : " << port.toStdString() << std::endl;
   OpenFlipper::Options::remoteControl(port.toInt());

 }

 return CommandLineOk;
}

int main(int argc, char **argv)
{

  // Remove -psn_0_xxxxx argument which is automatically
  // attached by MacOSX
  for (int i = 0; i < argc; i++) {
    if(strlen(argv[i]) > 4) {
      if( ( (argv[i])[0] == '-' ) &&
        ( (argv[i])[1] == 'p' ) &&
        ( (argv[i])[2] == 's' ) &&
        ( (argv[i])[3] == 'n' ) ) {
        argc--;
        argv[i] = (char *)"";
      }
    }
  }

  OpenFlipper::Options::argc(&argc);
  OpenFlipper::Options::argv(&argv);

  // Ugly command line parse to check if we run in batch mode or not.
  // Qt parser needs either QApplication or QCoreApplication to work.
  // But we need that option to decide which one to choose so ...
  for (int i = 1; i < argc; i++) {
     QString option = QString(argv[i]);
     if (option.contains("batch") || option.contains("-b") ||
         option.contains("--batch") || option.contains("/b") ||
         option.contains("/batch" )) {
       std::cerr << "Batch Mode started" << std::endl;
       OpenFlipper::Options::nogui(true);
     }

   }

  QCommandLineParser parser;
  QString errorMessage;


#ifndef NO_CATCH_SIGSEGV
  // Set a handler for segfaults
  std::signal(SIGSEGV, segfaultHandling);
#endif

  OpenFlipper::Options::windowTitle(TOSTRING(PRODUCT_STRING)" v" + OpenFlipper::Options::coreVersion());

  if ( !OpenFlipper::Options::nogui() ) {

    // OpenGL check
    QApplication::setColorSpec( QApplication::CustomColor );
    QApplication app(argc,argv);

    // Set organization and application names
    QCoreApplication::setOrganizationName("rwth-aachen.de");
    QCoreApplication::setApplicationName(TOSTRING(PRODUCT_STRING));
    QCoreApplication::setApplicationVersion(OpenFlipper::Options::coreVersion());

    if ( !QGLFormat::hasOpenGL() ) {
      std::cerr << "This system has no OpenGL support.\n";
      return -1;
    }

#ifndef __APPLE__
    glutInit(&argc,argv);
#endif

    // create core ( this also reads the ini files )
    Core * w = new Core( );


    switch (parseCommandLine(parser, &errorMessage)) {
      case CommandLineOk:
        break;
      case CommandLineError:
        fputs(qPrintable(errorMessage), stderr);
        fputs("\n\n", stderr);
        fputs(qPrintable(parser.helpText()), stderr);
        delete w;
        return 1;
      case CommandLineVersionRequested:
        printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
            qPrintable(QCoreApplication::applicationVersion()));
        return 0;
      case CommandLineHelpRequested:
        parser.showHelp();
        Q_UNREACHABLE();
    }

#ifdef WIN32
	//attach a console if necessary
	attachConsole();
#endif

    QString tLang = OpenFlipperSettings().value("Core/Language/Translation","en_US").toString();

    if (tLang == "locale")
      tLang = QLocale::system().name();

    // Install translator for qt internals
    QTranslator qtTranslator;
//     std::cerr << "Loading qt translations from: " << QLibraryInfo::location(QLibraryInfo::TranslationsPath).toStdString() << std::endl;
//     if ( qtTranslator.load("qt_" + tLang, QLibraryInfo::location(QLibraryInfo::TranslationsPath)) )
//       std::cerr << "Loaded" << std::endl;
//     std::cerr << "Loading qt translations from: " << QLibraryInfo::location(QLibraryInfo::TranslationsPath).toStdString() << std::endl;
    qtTranslator.load("qt_" + tLang, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
//       std::cerr << "Loaded" << std::endl;

    app.installTranslator(&qtTranslator);
    
    // install translator for Core Application
    QString translationDir = OpenFlipper::Options::translationsDirStr() + QDir::separator();

//     std::cerr << "Loading own translations from: " << QString(translationDir + " (" + tLang + ")").toStdString() << std::endl;

    QDir dir(translationDir);
    dir.setFilter(QDir::Files);

    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
      QFileInfo fileInfo = list.at(i);

      if ( fileInfo.baseName().contains(tLang) ){
        QTranslator* myAppTranslator = new QTranslator();

        if ( myAppTranslator->load( fileInfo.filePath() ) ){
//           std::cerr << "Loaded " << fileInfo.fileName().toStdString() << std::endl;
    
          app.installTranslator(myAppTranslator);
        } else {
  	  delete myAppTranslator;
        }
      }
     }
    
    // After setting all Options from command line, build the real gui
    w->init();

    #ifndef __APPLE__
    initGlew();
    #endif
 
    const QStringList positionalArguments = parser.positionalArguments();

    for ( auto file: positionalArguments ) {
      w->commandLineOpen(file, openPolyMeshes);
    }

    return app.exec();

  } else {

    QCoreApplication app(argc,argv);

    // Set organization and application names
    QCoreApplication::setOrganizationName("rwth-aachen.de");
    QCoreApplication::setApplicationName(TOSTRING(PRODUCT_STRING));
    QCoreApplication::setApplicationVersion(OpenFlipper::Options::coreVersion());

    // create widget ( this also reads the ini files )
    Core * w = new Core( );

    switch (parseCommandLine(parser, &errorMessage)) {
      case CommandLineOk:
        break;
      case CommandLineError:
        fputs(qPrintable(errorMessage), stderr);
        fputs("\n\n", stderr);
        fputs(qPrintable(parser.helpText()), stderr);
        delete w;
        return 1;
      case CommandLineVersionRequested:
        printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
            qPrintable(QCoreApplication::applicationVersion()));
        return 0;
      case CommandLineHelpRequested:
        parser.showHelp();
        Q_UNREACHABLE();
    }

    // After setting all Options from command line, build the real gui
    w->init();

    const QStringList positionalArguments = parser.positionalArguments();

    for ( auto file: positionalArguments ) {
      w->commandLineOpen(file, openPolyMeshes);
    }

    return app.exec();
  }

  return 0;
}
