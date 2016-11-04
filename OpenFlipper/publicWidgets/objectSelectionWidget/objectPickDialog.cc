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

//== INCLUDES =================================================================
#include <QHBoxLayout>
#include <QPushButton>
#include <QTreeView>
#include <QMouseEvent>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/widgets/glWidget/simpleViewer.hh>
#include <OpenFlipper/widgets/glWidget/QtBaseViewer.hh>

#include "objectPickDialog.hh"
#include "TreeModelObjectSelection.hh"

//== NAMESPACES ===============================================================

//=============================================================================
//
//  CLASS ObjectPickDialog - IMPLEMENTATION
//
//=============================================================================

/// Constructor
ObjectPickDialog::ObjectPickDialog(QStringList _flags, QStringList _types, bool _withGroups) :
  QDialog (),
  selectedId_(0)
{
  QHBoxLayout *hL = new QHBoxLayout;
  QHBoxLayout *bL = new QHBoxLayout;
  QVBoxLayout *vL = new QVBoxLayout;

  model_ = new TreeModelObjectSelection ();

  treeView_ = new QTreeView;
  treeView_->setModel (model_);
  treeView_->resizeColumnToContents (0);
  treeView_->setSelectionMode (QAbstractItemView::SingleSelection);
  treeView_->setSelectionBehavior (QAbstractItemView::SelectRows);

  viewer_ = new SimpleViewer ();
  viewer_->properties ().objectMarker (&marker_);

  okButton_ = new QPushButton (tr("OK"));
  cancelButton_ = new QPushButton (tr("Cancel"));

  connect (okButton_, SIGNAL (pressed()), this, SLOT (accept()));
  connect (cancelButton_, SIGNAL (pressed()), this, SLOT (reject()));

  hL->addWidget (viewer_);
  hL->setStretchFactor (viewer_, 1);
  hL->addWidget (treeView_);

  bL->addStretch (1);
  bL->addWidget (okButton_);
  bL->addWidget (cancelButton_);

  vL->addLayout(hL);
  vL->addLayout(bL);

  setLayout (vL);

  resize (700, 400);

  setWindowTitle(tr("Click on object or select from list..."));

  connect (treeView_, SIGNAL (activated( const QModelIndex& )),
           this, SLOT (activated(const QModelIndex&)));
  connect (viewer_->viewer(), SIGNAL (signalMouseEventClick(QMouseEvent*, bool)),
           this, SLOT (slotMouseEventClick(QMouseEvent*, bool)));

  for (PluginFunctions::BaseObjectIterator o_it; o_it != PluginFunctions::baseObjectsEnd(); ++o_it)  {

    bool ok = true;

    if (!_flags.empty ())
    {
      bool found = false;
      foreach (QString flag, _flags)
        if (o_it->flag (flag))
        {
          found = true;
          break;
        }

      if (!found)
        ok = false;
    }

    if (!_types.empty ())
    {
      if (!_types.contains (typeName (o_it->dataType())))
        ok = false;
    }

    if (o_it->isGroup() && !_withGroups)
      continue;

    if (ok)
    {
      if (!_withGroups)
        model_->objectAdded(o_it, PluginFunctions::objectRoot());
      else
        model_->objectAdded (o_it);
    }
  }
}

//------------------------------------------------------------------------------

/// Destructor
ObjectPickDialog::~ ObjectPickDialog()
{
  for (PluginFunctions::BaseObjectIterator o_it; o_it != PluginFunctions::baseObjectsEnd(); ++o_it)  {
    o_it->setFlag("vsi_objectId_selected", false);
  }

  delete model_;
}

//------------------------------------------------------------------------------

void ObjectPickDialog::activated(const QModelIndex & _index)
{
  if (_index.isValid()) {

    TreeItemObjectSelection *item = static_cast<TreeItemObjectSelection*>(_index.internalPointer());
    if (item)
    {
      selectedId (item->id());
    }
  }
}

//------------------------------------------------------------------------------

void ObjectPickDialog::slotMouseEventClick(QMouseEvent * _event, bool /*_double*/)
{
  size_t nodeIdx, targetIdx;


  if (viewer_->viewer()->pick(ACG::SceneGraph::PICK_ANYTHING, _event->pos(), nodeIdx, targetIdx))
  {
    BaseObjectData *obj = 0;
    if (PluginFunctions::getPickedObject (nodeIdx, obj))
    {
      if (!obj->flag ("vsi_objectId_disabled"))
      {
        selectedId (obj->id());
      }
    }
  }
}

//------------------------------------------------------------------------------

unsigned int ObjectPickDialog::selectedId()
{
  return selectedId_;
}

//------------------------------------------------------------------------------

void ObjectPickDialog::selectedId(unsigned int _id)
{
  BaseObject* obj = 0;

  if (PluginFunctions::getObject(_id, obj))
  {
    BaseObject* obj2 = 0;

    if (selectedId_ != _id && PluginFunctions::getObject(selectedId_, obj2))
    {
      obj2->setFlag ("vsi_objectId_selected", false);
      if (obj2->isGroup())
        setForGroup (obj2, "vsi_objectId_selected", false);
    }
    obj->setFlag ("vsi_objectId_selected", true);
    if (obj->isGroup())
      setForGroup (obj, "vsi_objectId_selected", true);

    selectedId_ = _id;
    viewer_->viewer()->updateGL ();
    treeView_->setCurrentIndex (model_->getModelIndex(_id, 0));
  }
}

//------------------------------------------------------------------------------

void ObjectPickDialog::setForGroup(BaseObject *_obj, QString _flag, bool _enabled)
{
  for (PluginFunctions::BaseObjectIterator o_it; o_it != PluginFunctions::baseObjectsEnd(); ++o_it)  {
    if (o_it->id () == _obj->id ())
      continue;
    if (o_it->isInGroup (_obj->id ()))
    {
      o_it->setFlag(_flag, _enabled);
      if (o_it->isGroup())
        setForGroup (o_it, _flag, _enabled);
    }
  }
}

//------------------------------------------------------------------------------



