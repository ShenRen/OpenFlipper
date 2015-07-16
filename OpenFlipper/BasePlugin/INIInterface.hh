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


#ifndef INIINTERFACE_HH
#define INIINTERFACE_HH

 #include <OpenFlipper/common/Types.hh>

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

 #include <QMenuBar>


 #include <OpenFlipper/INIFile/INIFile.hh>

 /** \brief Interface class for Plugins which have to store information in ini files
  *
  * Using this interface you can store or load additional information to an ini file on a
  * per object basis or settings for your plugin. After basic information is written to the
  * ini file by the core ( or loaded by the core ) the corresponding functions in this
  * Interface will be executed.
  * You get the object id for the object which has to be used. For every object the functions
  * will get called and you have to save your data only for the given object!\n
  * The ini file in the functions is already open. You may not close it!
  * Additionally there are two slots which are called once per plugin when writing a settings
  * file. These slots should be used to store information which is used by your plugin.
  *
  * See \ref dataFlow for a detailed overview of OpenFlipper's data flow and interface function calls.
 */
class INIInterface {
   public :

      /// Destructor
      virtual ~INIInterface() {};

   private slots:

    //=======================================
    /** @name Per object settings
    * @{ */
    //=======================================
      /** \brief Load per object settings
       *
       *  Every time the core opens a settings file containing objects
       *  the core will call this slot for each object it finds.
       *  The object itself is loaded before the slot is called. Therefore
       *  the object is available from within your plugin.
       *
       *  @param _ini ini file
       *  @param _id Id of the object to load data for
       */
      virtual void loadIniFile( INIFile& _ini ,int _id ) {};

      /** \brief Save per object settings
       *
       * Every time a settings file is created this slot is called
       * once per object. Here you can write additional information
       * handled by your plugin which is attached to the object.
       *
       * @param _ini ini file
       * @param _id Identifier of the object to save
       */
      virtual void saveIniFile( INIFile& _ini ,int _id) {};

    /** @} */


    //=======================================
    /** @name Plugin Options and Settings
    * @{ */
    //=======================================

    private slots:

      /** \brief Load per Plugin settings
       *
       *  When the core loads an ini file and it contains settings for
       *  Plugin or the core itself this slot will be
       *  called once per Plugin.
       *  This Slot will be called after loading the core settings and
       *  before objects are loaded
       *
       * @param _ini  ini file
       */
      virtual void loadIniFileOptions( INIFile& _ini ) {};

      /** \brief Load per Plugin settings after objects are loaded
       *
       *  When the core loads an ini file and it contains settings for
       *  Plugin or the core itself this slot will be
       *  called once per Plugin. In contrast to loadIniFileOptions
       *  this slot will be called after all objects are loaded
       *
       * @param _ini  ini file
       */
      virtual void loadIniFileOptionsLast( INIFile& _ini ) {};

      /** \brief Save Plugin Options
       *  When the core is about to save an ini file this slot will be
       *  called once per Plugin.
       *  This Slot will be called after saving the core settings and
       *  before objects are saved to the file
       *
       * @param _ini ini file
       */
      virtual void saveIniFileOptions( INIFile& _ini ) {};

      /** \brief Save per Plugin settings when application is about to quit
       *
       * Before the Core is closed, this slot will be called per plugin to
       * save per plugin settings.
       *
       * @param _ini ini file
       */
      virtual void saveOnExit( INIFile& _ini ) {};


   /** @} */


};

Q_DECLARE_INTERFACE(INIInterface,"OpenFlipper.INIInterface/1.2")

#endif // INIINTERFACE_HH
