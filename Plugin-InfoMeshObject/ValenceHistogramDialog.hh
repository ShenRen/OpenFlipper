/*
 * ValenceHistogramDialog.hh
 *
 *  Created on: Jan 27, 2016
 *      Author: hc
 */

#ifndef PLUGIN_INFOMESHOBJECT_VALENCEHISTOGRAMDIALOG_HH_
#define PLUGIN_INFOMESHOBJECT_VALENCEHISTOGRAMDIALOG_HH_

#include <ObjectTypes/PolyMesh/PolyMeshTypes.hh>
#include <ObjectTypes/TriangleMesh/TriangleMeshTypes.hh>

#include "ui_infoValenceHistograms.hh"

#if QT_VERSION >= 0x050000
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


class ValenceHistogramDialog: public QDialog, public Ui::ValenceHistogramsDlg {
    Q_OBJECT

    public:
        ValenceHistogramDialog(TriMesh &mesh, QWidget *parent = 0);
        ValenceHistogramDialog(PolyMesh &mesh, QWidget *parent = 0);

        template<class MeshT> void init(MeshT &mesh);

    private:
        std::vector<size_t> vertex_valence_hist, face_valence_hist;
};

#endif /* PLUGIN_INFOMESHOBJECT_VALENCEHISTOGRAMDIALOG_HH_ */
