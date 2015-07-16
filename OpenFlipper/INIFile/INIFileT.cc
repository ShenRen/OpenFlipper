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
//  CLASS INIFile Templates - IMPLEMENTATION
//
//=============================================================================

#define INIFILE_C

//== INCLUDES =================================================================

#include "INIFile.hh"

//std include
#include <fstream>
#include <iostream>
#include <functional>
#include <sstream>
#include <cstring>
#include <cctype>
//#include <ios>

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>

//== IMPLEMENTATION ==========================================================

//! Get a Vec_n_i
template < typename VectorT >
bool
INIFile::
get_entryVeci ( VectorT & _val ,
                const QString & _section,
                const QString & _key ) const
{
  SectionMap::const_iterator sIter;
  EntryMap::const_iterator eIter;

  // does the given section exist?
  if( (sIter = m_iniData.find( _section )) == m_iniData.end() )
    return false;

  // does the given entry exist?
  if( (eIter = sIter->second.find( _key )) == sIter->second.end() )
    return false;

  QStringList list = eIter->second.split(';',QString::SkipEmptyParts);

  // get dimension of requested vector
  VectorT tmp;
  int dim = tmp.dim();

  if ( list.size() != dim ) {
    std::cerr << "Differet size when reading Vector" << std::endl;
    return false;
  }

  bool ok = true;
  for ( int i = 0 ; i < dim; ++i) {
    bool tmpOk = false;
    _val[i] = (typename VectorT::value_type) list[i].toInt(&tmpOk);
    ok &= tmpOk;
  }

  return ok;
}

// -----------------------------------------------------------------------------

//! Get a Vec_n_d
template < typename VectorT >
bool
INIFile::
get_entryVecd ( VectorT & _val ,
                const QString & _section,
                const QString & _key ) const
{
  SectionMap::const_iterator sIter;
  EntryMap::const_iterator eIter;

  // does the given section exist?
  if( (sIter = m_iniData.find( _section )) == m_iniData.end() )
    return false;

  // does the given entry exist?
  if( (eIter = sIter->second.find( _key )) == sIter->second.end() )
    return false;

  QStringList list = eIter->second.split(';',QString::SkipEmptyParts);

  // get dimension of requested vector
  VectorT tmp;
  int dim = tmp.dim();

  if ( list.size() != dim ) {
    std::cerr << "Differet size when reading Vector" << std::endl;
    return false;
  }

  bool ok = true;
  for ( int i = 0 ; i < dim; ++i) {
    bool tmpOk = false;
    _val[i] = (typename VectorT::value_type) list[i].toDouble(&tmpOk);
    ok &= tmpOk;
  }

  return ok;
}

// -----------------------------------------------------------------------------

//! Get a Vec_n_f
template < typename VectorT >
bool
INIFile::
get_entryVecf ( VectorT & _val ,
                const QString & _section,
                const QString & _key ) const
{
  SectionMap::const_iterator sIter;
  EntryMap::const_iterator eIter;

  // does the given section exist?
  if( (sIter = m_iniData.find( _section )) == m_iniData.end() )
    return false;

  // does the given entry exist?
  if( (eIter = sIter->second.find( _key )) == sIter->second.end() )
    return false;

  QStringList list = eIter->second.split(';',QString::SkipEmptyParts);

  // get dimension of requested vector
  VectorT tmp;
  int dim = tmp.dim();

  if ( list.size() != dim ) {
    std::cerr << "Differet size when reading Vector" << std::endl;
    return false;
  }

  bool ok = true;
  for ( int i = 0 ; i < dim; ++i) {
    bool tmpOk = false;
    _val[i] = (typename VectorT::value_type) list[i].toFloat(&tmpOk);
    ok &= tmpOk;
  }

  return ok;
}

// -----------------------------------------------------------------------------

//! Addition of a vector of Vec_n_whatever
template < typename VectorT >
void
INIFile::
add_entryVec ( const QString & _section,
               const QString & _key,
               const VectorT  & _value)
{

  // get dimension of stored vectors
  VectorT tmp;
  int dim = tmp.dim();

  QString list;
  for (int j = 0; j < dim; ++j)
    list +=  QString::number( _value[j] ) + ";";

  m_iniData[ _section ][ _key ] = list;
}

// -----------------------------------------------------------------------------

template < typename VectorT >
bool
INIFile::
get_entryVecd ( std::vector< VectorT > & _val ,
                const QString & _section,
                const QString & _key ) const
{
  SectionMap::const_iterator sIter;
  EntryMap::const_iterator eIter;

  _val.clear();

  // does the given section exist?
  if( (sIter = m_iniData.find( _section )) == m_iniData.end() )
    return false;

  // does the given entry exist?
  if( (eIter = sIter->second.find( _key )) == sIter->second.end() )
    return false;

  QStringList list = eIter->second.split(';',QString::SkipEmptyParts);

  // get dimension of stored vectors
  VectorT tmp;
  int dim = tmp.dim();

  bool ok = true;
  for ( int i = 0 ; i < list.size(); )
  {
    if ( list[i].isEmpty() )
      continue;
    bool tmpOk = false;

    std::vector<double> tmp;

    for (int j = 0; j < dim; ++j)
    {
      // check data type
      tmp.push_back( list[i].toDouble(&tmpOk) );
      ++i;
    }

    VectorT vec;
    for (int j = 0; j < dim; ++j)
      vec[j] = (typename VectorT::value_type)(tmp[j]);

    _val.push_back(vec);
    ok &= tmpOk;
  }

  return ok;
}


// -----------------------------------------------------------------------------

template < typename VectorT >
bool
INIFile::
get_entryVecf ( std::vector< VectorT > & _val ,
                const QString & _section,
                const QString & _key ) const
{
  SectionMap::const_iterator sIter;
  EntryMap::const_iterator eIter;

  _val.clear();

  // does the given section exist?
  if( (sIter = m_iniData.find( _section )) == m_iniData.end() )
    return false;

  // does the given entry exist?
  if( (eIter = sIter->second.find( _key )) == sIter->second.end() )
    return false;

  QStringList list = eIter->second.split(';',QString::SkipEmptyParts);

  // get dimension of stored vectors
  VectorT tmp;
  int dim = tmp.dim();

  bool ok = true;
  for ( int i = 0 ; i < list.size(); )
  {
    if ( list[i].isEmpty() )
      continue;
    bool tmpOk = false;

    std::vector<double> tmp;

    for (int j = 0; j < dim; ++j)
    {
      // check data type
      tmp.push_back( list[i].toFloat(&tmpOk) );
      ++i;
    }

    VectorT vec;
    for (int j = 0; j < dim; ++j)
      vec[j] = (typename VectorT::value_type)(tmp[j]);

    _val.push_back(vec);
    ok &= tmpOk;
  }

  return ok;
}


// -----------------------------------------------------------------------------

template < typename VectorT >
bool
INIFile::
get_entryVeci ( std::vector< VectorT > & _val ,
                const QString & _section,
                const QString & _key ) const
{
  SectionMap::const_iterator sIter;
  EntryMap::const_iterator eIter;

  _val.clear();

  // does the given section exist?
  if( (sIter = m_iniData.find( _section )) == m_iniData.end() )
    return false;

  // does the given entry exist?
  if( (eIter = sIter->second.find( _key )) == sIter->second.end() )
    return false;

  QStringList list = eIter->second.split(';',QString::SkipEmptyParts);

  // get dimension of stored vectors
  VectorT tmp;
  int dim = tmp.dim();

  bool ok = true;
  for ( int i = 0 ; i < list.size(); )
  {
    if ( list[i].isEmpty() )
      continue;
    bool tmpOk = false;

    std::vector<double> tmp;

    for (int j = 0; j < dim; ++j)
    {
      // check data type
      tmp.push_back( list[i].toInt(&tmpOk) );
      ++i;
    }

    VectorT vec;
    for (int j = 0; j < dim; ++j)
      vec[j] = (typename VectorT::value_type)(tmp[j]);

    _val.push_back(vec);
    ok &= tmpOk;
  }

  return ok;
}


// -----------------------------------------------------------------------------


template < typename VectorT >
void
INIFile::
add_entryVec ( const QString & _section,
               const QString & _key,
               const std::vector< VectorT > & _value)
{
  QString list;
  typename std::vector< VectorT >::const_iterator viter;

  VectorT tmp;

  for(viter = _value.begin();viter!=_value.end();++viter)
  {
    for (int i = 0; i < tmp.dim(); ++i)
      list += QString::number( (*viter)[i] ) + ";";
  }

  m_iniData[ _section ][ _key ] = list;
}
