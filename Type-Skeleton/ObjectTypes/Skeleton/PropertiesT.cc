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

#define PROPERTIES_C

#include <cassert>
#include <utility>
#include <iostream>


//-----------------------------------------------------------------------------

/**
 * @brief Adds a new property
 *
 * Give the property type as template parameter of the property handle. Assign a unique name to the property,
 * to be able to access it later. The property is created an the property handle is updated. After this call
 * it is directly ready for use. Returns false if a property with this name already exists. In that case
 * the property handle will point to that property.
 *
 * @param _hProp A new unused property handle. When the method returns it will always point to a property.
 * @param _name The properties name, has to be unique.
 * @return true if a new property was created, false if an existing is returned.
 */
template<typename T>
bool Properties::add_property(PropertyHandleT<T> &_hProp, std::string _name)
{
  // return the property if it exists
  if(get_property(_hProp, _name))
    return false;

  // create it if not
  // find the first free entry
  std::vector<BaseProperty*>::iterator it;
  for(it = properties_.begin(); it != properties_.end(); ++it)
    if(*it == 0)
      break;

  if(it == properties_.end())	// found a free entry?
  {
    // append at the end
    _hProp.idx_ = properties_.size();
    property_names_.insert( std::pair<std::string, int>(_name, properties_.size()) );
    properties_.push_back(dynamic_cast<BaseProperty*>(new PropertyT<T>(size_)));		// the new one needs to have the same number of entries, to keep the correspondence
  }else{
    // insert in the free spot
    _hProp.idx_ = it - properties_.begin();
    property_names_.insert( std::pair<std::string, int>(_name, _hProp.idx_) );
    *it = new PropertyT<T>(size_);
  }
  return true;
}

//-----------------------------------------------------------------------------

/**
 * @brief Initiates the property handle
 *
 * If this call is successful and there is a property with the given name, the property handle will point to
 * it. If not, this method returns false.
 *
 * @param _hProp A new, unused property handle
 * @param _name The properties name
 * @return true if the property exists and _hProp is now valid, false otherwise
 */
template<typename T>
bool Properties::get_property(PropertyHandleT<T> &_hProp, std::string _name)
{
  // find the name
  std::map<std::string, int>::iterator f = property_names_.find(_name);
  if(f == property_names_.end())
    return false;

  // return the index
  _hProp.idx_ = f->second;
        assert(properties_[_hProp.idx_] != 0);
  return true;
}

//-----------------------------------------------------------------------------

/**
 * @brief Deletes a property including all values
 *
 * Call this method to remove the property identified by the handle.
 *
 * @param _hProp A valid property handle pointing to a property
 * @return true if the handle was valid and a property was deleted, false otherwise
 */
template<typename T>
bool Properties::remove_property(PropertyHandleT<T> &_hProp)
{
  if(!_hProp.isValid())
    return false;
  assert(_hProp.idx_ < (int)properties_.size());
  assert(properties_[_hProp.idx_] != 0);

  // delete the property
  delete properties_[_hProp.idx_];
  properties_[_hProp.idx_] = 0;

  // find the names entry and delete it
  for(std::map<std::string, int>::iterator it = property_names_.begin(); it != property_names_.end(); ++it)
  {
    if(it->second == _hProp.idx_)
    {
      property_names_.erase(it);
      break;
    }
  }

  // invalidate the handle
  _hProp.idx_ = -1;

  return true;
}

//-----------------------------------------------------------------------------

/**
 * @brief Direct access to the properties values
 *
 * Access the property by using the same index as you use for the corresponding object in the derived class.
 * This method returns a reference to the type given by the property handle. Make sure you use the right
 * template parameter, otherwise the result is undefined (most likely a zero-reference).
 *
 * @param _hProp A valid handle to a property
 * @param _index The index of the property, corresponds to the index of the object in the derived class
 * 				 this property refers to
 */
template<typename T>
T &Properties::property(PropertyHandleT<T> &_hProp, int _index)
{
  assert(_hProp.idx_ >= 0 && _hProp.idx_ < (int)properties_.size());
  assert(properties_[_hProp.idx_] != 0);
  assert(reinterpret_cast< PropertyT<T>* >(properties_[_hProp.idx_]) != 0);

  return (*reinterpret_cast< PropertyT<T>* >(properties_[_hProp.idx_]))[_index];
}

//-----------------------------------------------------------------------------
//   PropertyHandleT
//-----------------------------------------------------------------------------

/**
 * @brief Constructs a new property handle
 *
 * Do not use the index parameter, unless you really know what you do. Rather pass 0 and then
 * use Properties::get_property.
 *
 * @param _idx The properties internal index, use with caution (or better, don't use at all)
 */
template<typename T>
PropertyHandleT<T>::PropertyHandleT(int _idx) :
  idx_(_idx)
{
}

//-----------------------------------------------------------------------------

/**
 * @brief Destructor
 */
template<typename T>
PropertyHandleT<T>::~PropertyHandleT()
{
}

//-----------------------------------------------------------------------------

/**
 * @brief Returns true if the handle is valid, false otherwise
 *
 * This method does not test if there is a property with that index, so make sure you don't use
 * handles after their property was deleted or use invalid values in the constructor.
 */
template<typename T>
bool PropertyHandleT<T>::isValid()
{
  return idx_ >= 0;
}

//-----------------------------------------------------------------------------
//       PropertyT
//-----------------------------------------------------------------------------

/**
 * @brief Creates a new property with the given size
 *
 * If a property is created when the derived class is already holding objects, it is of course not in sync
 * with regard to the number of stored values (e.g. see Properties::PropertyT::insert_at). The \e _size
 * parameter allocates the memory to get the new property in sync.
 */
template<typename T>
Properties::PropertyT<T>::PropertyT(unsigned long _size)
{
  for(unsigned long i = 0; i < _size; ++i)
    values_.push_back(T());
}

//-----------------------------------------------------------------------------

/**
 * @brief Destructor
 */
template<typename T>
Properties::PropertyT<T>::~PropertyT()
{
}

//-----------------------------------------------------------------------------

/**
 * @brief Direct access to the value with the given index
 *
 * This method is used by the Properties class to gain access to the Properties values
 */
template<typename T>
T &Properties::PropertyT<T>::operator[](int _index)
{
  assert(_index >= 0 && _index < (int)values_.size());

  return values_[_index];
}

//-----------------------------------------------------------------------------

/**
 * @brief Insert element
 */
template<typename T>
void Properties::PropertyT<T>::insert_at(int _index)
{
  assert(_index >= 0 && _index <= (int)values_.size());

  values_.insert(values_.begin() + _index, T());
}

//-----------------------------------------------------------------------------

/**
 * @brief Remove element
 */
template<typename T>
void Properties::PropertyT<T>::remove_at(int _index)
{
  assert(_index >= 0 && _index < (int)values_.size());

  values_.erase(values_.begin() + _index);
}

//-----------------------------------------------------------------------------

/**
 * @brief Clear the property
 */
template<typename T>
void Properties::PropertyT<T>::clear()
{
  values_.clear();
}

//-----------------------------------------------------------------------------

