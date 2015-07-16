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




//=============================================================================
//
//  Types
//
//=============================================================================

/**
 * \file Types.cc
 * This File contains the basic functions to handle datatypes
 */


//== INCLUDES =================================================================

#include "TypesInternal.hh"
#include "DataTypes.hh"
#include <map>

#include <OpenFlipper/common/GlobalOptions.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>

#include <QCoreApplication>
#include <QMetaType>


/** This field defines the start id for custom datatypes. It starts high to avoid conflicts with previously
 * hardcoded versions.
 */
static int nextTypeId_ = 2;

/** This map maps an dataType id to an typeName
 */
static std::map< DataType, QString > typeToString;

/** This map maps an dataType name to its id in the types vector
 */
static std::map< QString , size_t > stringToTypeInfo;

/** This map maps an dataType id to its id in the types vector
 */
static std::map< DataType , size_t > typeToTypeInfo;

static QIcon dummyIcon;

static std::vector< TypeInfo > types;

//== Functions =========================================================

std::ostream &operator<<(std::ostream &stream, DataType type)
{
  stream << type.value() ;
  
  return stream;
}

void initializeTypes() {
  stringToTypeInfo["Unknown"] = types.size();
  DataType test(DATA_UNKNOWN);
  typeToTypeInfo[test]   = types.size();
  types.push_back( TypeInfo(DATA_UNKNOWN            ,"Unknown"        ,"Unknown.png", QCoreApplication::translate("Types","Unknown")) );

  stringToTypeInfo["Group"]  = types.size();
  typeToTypeInfo[DATA_GROUP] = types.size();
  types.push_back( TypeInfo(DATA_GROUP           ,"Group"          ,"group.png", QCoreApplication::translate("Types","Group")) );

  stringToTypeInfo["All"]  = types.size();
  typeToTypeInfo[DATA_ALL] = types.size();
  types.push_back( TypeInfo(DATA_ALL             ,"All"            ,"Unknown.png", QCoreApplication::translate("Types","All")) );

  typeToString[DATA_UNKNOWN] = "Unknown";
  typeToString[DATA_GROUP] = "Group";
  typeToString[DATA_ALL] = "All";
  
  // Preload the static icons
  setTypeIcon(DATA_GROUP,"group.png");
}

/// Adds a datatype and returns the id for the new type
DataType addDataType(QString _name, QString _readableName) {
  
  // Check if datatype already exists.
  // If so, we return the typeId that is used for it
  if ( typeExists(_name) ) {
    std::cerr << "Redefinition of existing data type!" << std::endl;
    return typeId(_name);
  }
  
  int type = nextTypeId_;

  stringToTypeInfo[ _name ] = types.size();
  typeToTypeInfo[ type ] = types.size();
  types.push_back( TypeInfo(type, _name, "Unknown.png", _readableName ));

  typeToString[type] = _name;

  nextTypeId_ *= 2;
  return( type );
}

/// Get the id of a type with given name
DataType typeId(QString _name) {

  std::map<QString, size_t>::iterator index = stringToTypeInfo.find( _name );

  if ( index != stringToTypeInfo.end() )
    return types[ index->second ].type;
  else {
    #ifdef DEBUG
    std::cerr << "Unknown Data type with name " << _name.toStdString() << std::endl;
    #endif
    return DATA_UNKNOWN;
  }
}

/// Get the name of a type with given id
QString typeName(DataType _id) {

  std::map<DataType, QString>::iterator name = typeToString.find(_id);

  if ( name != typeToString.end() )
    return name->second;
  else {
    #ifdef DEBUG
    std::cerr << "Unable to retrieve typeName for id " << _id << std::endl;
    #endif
    return "Unknown";
  }
}

/// Check if a type with the given name exists
bool typeExists( QString _name ) {
  return ( stringToTypeInfo.find( _name ) !=  stringToTypeInfo.end() );
}


/// Return the number of registered types
size_t typeCount() {
  return types.size();
}

/// Get iterator pointing to the first element in the tyoes list
std::vector< TypeInfo >::const_iterator typesBegin() {
    return types.begin();
}

/// Get iterator pointing to the last element in the tyoes list
std::vector< TypeInfo >::const_iterator typesEnd() {
    return types.end();
}

/// Get the icon of a given dataType
QString typeIconName(QString  _name) {

  std::map<QString, size_t>::iterator index = stringToTypeInfo.find( _name );

  if ( index != stringToTypeInfo.end() )
    return types[ index->second ].iconName;
  else
    return "Unknown.png";
}

/// get the icon of a given dataType
QString typeIconName(DataType _id) {

  std::map<DataType, size_t>::iterator index = typeToTypeInfo.find(_id);

  if ( index != typeToTypeInfo.end() )
    return types[ index->second ].iconName;
  else
    return "Unknown.png";
}

/// get the icon of a given dataType
QIcon& typeIcon(DataType _id) {
  
  std::map<DataType, size_t>::iterator index = typeToTypeInfo.find(_id);
  
  if ( index != typeToTypeInfo.end() )
    return types[ index->second ].icon;
  else
    return dummyIcon;
}

/// Set the icon for a given dataType
void setTypeIcon( DataType _id   , QString _icon ) {

  if (  OpenFlipper::Options::gui() ) {
    std::map<DataType, size_t>::iterator index = typeToTypeInfo.find(_id);

    if ( index != typeToTypeInfo.end() ) {
      types[ index->second ].iconName = _icon;
      types[ index->second ].icon = QIcon( OpenFlipper::Options::iconDirStr() + QDir::separator() + _icon );
    } else
      std::cerr << "Could not set icon for DataType. Type not found!" << std::endl;
  }
}

/// Set the icon for a given dataType
void setTypeIcon( QString  _name , QString _icon ) {

  if (  OpenFlipper::Options::gui() ) {
    std::map<QString, size_t>::iterator index = stringToTypeInfo.find( _name );

    if ( index != stringToTypeInfo.end() ) {
      types[ index->second ].iconName = _icon;
      types[ index->second ].icon = QIcon( OpenFlipper::Options::iconDirStr() + QDir::separator() + _icon );
    } else
      std::cerr << "Could not set icon for DataType. Type not found!" << std::endl;
  }
}


/// Get DataType Human readable name ( this name might change. Use the typeName insted! )
QString dataTypeName( DataType _id ) {

  std::map<DataType, size_t>::iterator index = typeToTypeInfo.find(_id);

  if ( index != typeToTypeInfo.end() )
    return types[ index->second ].readableName ;
  else
    std::cerr << "Could not get human name for DataType. Type not found!" << std::endl;

  return QString(QCoreApplication::translate("Types","Unknown Type"));
}

/// Get DataType Human readable name ( this name might change. Use the typeName insted! )
QString dataTypeName( QString  _typeName ) {

  std::map<QString, size_t>::iterator index = stringToTypeInfo.find( _typeName );

  if ( index != stringToTypeInfo.end() )
    return types[ index->second ].readableName ;
  else
    std::cerr << "Could not get human name for DataType. Type not found!" << std::endl;

  return QString(QCoreApplication::translate("Types","Unknown Type"));
}



/// Set the icon for a given dataType
void setDataTypeName( DataType _id   , QString _name ) {

  std::map<DataType, size_t>::iterator index = typeToTypeInfo.find(_id);

  if ( index != typeToTypeInfo.end() )
    types[ index->second ].readableName = _name;
  else
    std::cerr << "Could not set human name for DataType. Type not found!" << std::endl;
}

/// Set the icon for a given dataType
void setDataTypeName( QString  _typeName , QString _name ) {

  std::map<QString, size_t>::iterator index = stringToTypeInfo.find( _typeName );

  if ( index != stringToTypeInfo.end() )
    types[ index->second ].readableName = _name;
  else
    std::cerr << "Could not set human name for DataType. Type not found!" << std::endl;
}


DataType::DataType():
  field(0)
{
};

DataType::DataType(const unsigned int& _i):
  field(_i)
{
};

//===========================================

bool DataType::operator!=( const unsigned int& _i ) const{
  return (_i != field); 
}

bool DataType::operator!=( const DataType& _i ) const{
  return (field != _i.field); 
}    

//===========================================

bool DataType::operator==( const unsigned int& _i ) const {
  return (_i == field); 
}

bool DataType::operator==(  const DataType& _i ) const{
  return (_i.field == field); 
}

//===========================================

DataType& DataType::operator=( const unsigned int& _i ) {
  field = _i;
  return (*this);
}

DataType& DataType::operator=( const DataType& _i ) {
  field = _i.field;
  return (*this);
}    

//===========================================

bool DataType::operator<( const unsigned int& _i ) const {
  return (field < _i); 
}

bool DataType::operator<( const DataType& _i ) const {
  return (field < _i.field); 
}

//===========================================

bool DataType::operator&( const unsigned int& _i ) const {
  return (field & _i); 
}

bool DataType::operator&( const DataType& _i ) const {
  return (field & _i.field); 
}    

//===========================================

DataType DataType::operator!() {
  DataType inv = (*this);
  inv.field = !inv.field;
  return inv; 
}

//===========================================

bool DataType::contains( const DataType& _i )const{
  //its not magic
  return ( (_i.field & field) == _i.field);
}

//===========================================

DataType& DataType::operator|=( const unsigned int& _i ) {
  field |= _i;
  return (*this);
}

DataType& DataType::operator|=( const DataType& _i )  {
  field |= _i.field ;
  return (*this);
}

//===========================================

DataType DataType::operator|( const DataType& _i ) const {
  return (field | _i.field); 
}    

//===========================================

DataType DataType::operator++(int /*_unused*/) {
  return (field *= 2);
}

//===========================================

DataType& DataType::operator++() {
  field *= 2;
  return (*this);
}  

//===========================================

unsigned int DataType::value() const {
  return( field );
}

QString DataType::name() const {
  return typeName(field);
}

//=============================================================================

void registerTypes() {
  qRegisterMetaType<IdList>("IdList");
  qRegisterMetaType<DataType>("DataType");
  qRegisterMetaType< QVector< int > >("QVector<int>");
  qRegisterMetaType<Vector>("Vector");
  qRegisterMetaType<Vector4>("Vector4");
  qRegisterMetaType<Matrix4x4>("Matrix4x4");
  qRegisterMetaType<UpdateType>("UpdateType");
  qRegisterMetaType<Logtype>("LogType");
}

//=============================================================================
//=============================================================================
