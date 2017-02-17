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
*   $Revision: 10745 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2011-01-26 10:23:50 +0100 (Mi, 26. Jan 2011) $                     *
*                                                                            *
\*===========================================================================*/

#include "Properties.hh"

using namespace std;

Properties::BaseProperty::BaseProperty()
{
}

Properties::BaseProperty::~BaseProperty()
{
}

Properties::Properties()
{
	size_ = 0;
}

Properties::~Properties()
{
	clear_properties();
}

/**
 * @brief While preserving the properties themself, all values in the property are deleted
 *
 * This method preserves the properties, the property handles remain valid. Call this method
 * if all objects in the derived class are deleted, so there is no object left that has properties
 * attached to it. Notice the difference to Properties::clear_properties.
 */
void Properties::clean_properties()
{
	for(vector<BaseProperty*>::iterator it = properties_.begin(); it != properties_.end(); ++it)
		if((*it) != 0)
			(*it)->clear();
	size_ = 0;
}

/**
 * @brief Deletes all properties, including their values
 *
 * Notice the difference to Properties::clean_properties, which preserves the properties and only
 * deletes the values inside them.
 */
void Properties::clear_properties()
{
	for(vector<BaseProperty*>::iterator it = properties_.begin(); it != properties_.end(); ++it)
		if((*it) != 0)
			delete *it;
	properties_.clear();
	property_names_.clear();
	size_ = 0;
}

/**
 * @brief Inserts a property for a new object at the given index
 *
 * If the derived class inserts a new object at the given index, the properties have to insert a new
 * property, to keep it in sync. This is done by calling this method. To append at the end simply
 * give the size of the vector holding the objects as parameter.
 *
 * @param _index Index of the new object, all the property indices for objects with index >= _index
 * 				 are increased by one
 */
void Properties::insert_property_at(int _index)
{
	size_ += 1;

	for(vector<BaseProperty*>::iterator it = properties_.begin(); it != properties_.end(); ++it)
		if(*it != 0)
			(*it)->insert_at(_index);
}

/**
 * @brief Removes a property for an object that is being deleted from the derived class
 *
 * The same as for Properties::insert_property_at, just the other way around.
 *
 * @param _index Index of the new object, all the property indices for objects with index >= _index
 * 				 are decremented by one
 */
void Properties::remove_property_at(int _index)
{
	size_ -= 1;

	for(vector<BaseProperty*>::iterator it = properties_.begin(); it != properties_.end(); ++it)
		if(*it != 0)
			(*it)->remove_at(_index);
}

/**
 * @brief Returns true if a property with the given name exists
 *
 * @param _name The properties name
 * @return true if the property exists, false otherwise
 */
bool Properties::has_property(std::string _name)
{
	if(property_names_.find(_name) == property_names_.end())
		return false;
	return true;
}
