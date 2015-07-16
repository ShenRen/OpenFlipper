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
*   $Revision: 11147 $                                                       *
*   $LastChangedBy: kremer $                                                 *
*   $Date: 2011-03-16 16:01:09 +0100 (Wed, 16 Mar 2011) $                    *
*                                                                            *
\*===========================================================================*/

#ifndef INFORMATIONINTERFACE_HH_
#define INFORMATIONINTERFACE_HH_

#include <OpenFlipper/common/Types.hh>

/** \file InformationInterface.hh
*
* Interface that provides means to
* get information on specific data types in OpenFlipper. \ref informationInterfacePage
*/


 /** \brief Interface class for providing information on objects
 *
 * \n
 * \ref informationInterfacePage "Detailed description"
 * \n
 *
 */

class InformationInterface {

public:
    /// Default destructor
    virtual ~InformationInterface() {};

public slots:

   /**  \brief Get data type for information requests
     *
     *  This slot is called by the core in order to identify
     *  the data types for which the plugin provides information.
     *
     *  @return The supported data types (OR'ed in case of multiple types)
     */
    virtual DataType supportedDataTypes() = 0;


    /**  \brief Show information dialog on clicked object
      *
      *  This slot is called each time the user clicks on an object.
      *  The information plugin for a specific type should display
      *  the necessary information on all entity types and the clicked
      *  object itself.
      *
      *  @param _clickedPoint The 2D point in screen coords, which the user has clicked
      *  @param _type         Data type to be registered
    */
    virtual void slotInformationRequested(const QPoint _clickedPoint, DataType _type) = 0;
};

/** \page informationInterfacePage Information Interface
\n
\image html informationInterface.png
\n

Click InformationInterface for a detailed overview of all provided slots
in the interface.

Derive from the information interface in order to provide information
on objects of a specific data type. To clarify this, we will give a short
example. Assuming we want to provide information on all mesh objects
(namely objects of type DATA_TRIANGLE_MESH and DATA_POLY_MESH).
In our plugin, we first have to override the slot that fetches the
supported data types:

\code
DataType MyInfoPlugin::supportedDataTypes() {

    return DataType(DATA_TRIANGLE_MESH | DATA_POLY_MESH);
}
\endcode

It's as simple as that. Now, each time an object is clicked in
identifier mode, slot slotInformationRequested() is called.
We ovveride this slot in order to do the actual picking
and displaying the gathered information in a dialog box:

\code
void MyInfoPlugin::slotInformationRequested(const QPoint _clickedPoint, const DataType _type) {

    // We only respond to requests for our data type
    if((_type != DATA_TRIANGLE_MESH) && (_type != DATA_POLY_MESH)) return;

    // Do the picking...

    // Display dialog box with all the information...
}
\endcode

*/

Q_DECLARE_INTERFACE(InformationInterface,"OpenFlipper.InformationInterface/1.0")

#endif /* INFORMATIONINTERFACE_HH_ */
