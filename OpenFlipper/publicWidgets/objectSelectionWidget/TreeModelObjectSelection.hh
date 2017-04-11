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

#ifndef TREEMODELOBJECTSELECTION_H
#define TREEMODELOBJECTSELECTION_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "TreeItemObjectSelection.hh"

class TreeModelObjectSelection : public QAbstractItemModel
{
  Q_OBJECT

  signals:
    // the connected TreeView changed data
    void dataChangedInside(int _id, int _column, const QVariant& _value);       

  public:

    /// Constructor
    explicit TreeModelObjectSelection(QObject *_parent = 0);

    /// Destructor
    ~TreeModelObjectSelection();

//===========================================================================
/** @name inherited from QAbstractItemModel
  * @{ */
//===========================================================================

public:

    /// Get the data of the corresponding entry
    QVariant data(const QModelIndex &index, int role) const;

    /// return the types of the corresponding entry
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /// return the header data of the model
    QVariant headerData(int section, 
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    /// Get the ModelIndex at given row,column
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;

    /// Get the parent ModelIndex
    QModelIndex parent(const QModelIndex &index) const;

    /// get the number of rows
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /** \brief Return the number of columns
     *
     * @param _parent unused
     * @return return always 2
     */
    int columnCount(const QModelIndex &_parent = QModelIndex()) const;

    /** \brief Set Data at 'index' to 'value'
     *
     * @param index a ModelIndex defining the position in the model
     * @param value the new value
     * @param role unused
     * @return return if the data was set successfully
     */
    bool setData(const QModelIndex &index, const QVariant &value , int role);

/** @} */

//===========================================================================
/** @name Internal DataStructure (the TreeItemObjectSelection Tree)
  * @{ */
//===========================================================================

public:

    /// Return the ModelIndex corresponding to a given TreeItemObjectSelection and Column
    QModelIndex getModelIndex(TreeItemObjectSelection* _object, int _column );

    /// Return the ModelIndex corresponding to a given object id and Column
    QModelIndex getModelIndex(int _id, int _column );

    /// Check if the given item is the root item
    bool isRoot(TreeItemObjectSelection* _item);

    /// Get the name of a given object
    bool getObjectName(TreeItemObjectSelection* _object , QString& _name);

    /// Get the TreeItemObjectSelection corresponding to a given ModelIndex
    TreeItemObjectSelection *getItem(const QModelIndex &index) const;

    /// Get the name of a TreeItemObjectSelection corresponding to a given ModelIndex
    QString itemName(const QModelIndex &index) const;

    /// Get the id of a TreeItemObjectSelection corresponding to a given ModelIndex
    int itemId(const QModelIndex &index) const;


    /// The object with the given id has been changed. Check if model also has to be changed
    void objectChanged(int id_);

    /// The object with the given id has been added. add it to the internal tree
    void objectAdded(BaseObject* _object);

     /// The object with the given id has been added. add it to the internal tree
    void objectAdded(BaseObject* _object, BaseObject* _parent);

    /// The object with the given id has been deleted. delete it from the internal tree
    void objectDeleted(int id_);

    /// move the item to a new parent
    void moveItem(TreeItemObjectSelection* _item, TreeItemObjectSelection* _parent );
private:

    /// Rootitem of the tree
    TreeItemObjectSelection* rootItem_;


    void propagateUpwards(TreeItemObjectSelection* _obj,   int _column, bool _value );
    void propagateDownwards(TreeItemObjectSelection* _obj, int _column );

/** @} */


};

#endif

