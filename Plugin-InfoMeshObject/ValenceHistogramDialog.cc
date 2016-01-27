/*
 * ValenceHistogramDialog.cc
 *
 *  Created on: Jan 27, 2016
 *      Author: hc
 */

#include "ValenceHistogramDialog.hh"


ValenceHistogramDialog::ValenceHistogramDialog(TriMesh &mesh, QWidget *parent)
    : QDialog(parent) {
    setupUi(this);
    init(mesh);
}

ValenceHistogramDialog::ValenceHistogramDialog(PolyMesh &mesh, QWidget *parent)
    : QDialog(parent) {
    setupUi(this);
    init(mesh);
}

static void fillHistogramTable(std::vector<size_t> &hist, QTableWidget &tw) {
    tw.clear();

    std::vector<size_t>::iterator nonzero_begin = hist.begin();
    for (; nonzero_begin != hist.end() && *nonzero_begin == 0;
            ++nonzero_begin);
    if (nonzero_begin == hist.end()) return;

    std::vector<size_t>::iterator nonzero_end = hist.end();
    for (; (nonzero_end-1) != nonzero_begin && *(nonzero_end-1) == 0;
            --nonzero_end);

    const size_t hist_size = std::distance(nonzero_begin, nonzero_end);
    const size_t ofs = std::distance(hist.begin(), nonzero_begin);

    tw.setRowCount(1);
    tw.setColumnCount(hist_size);

    QStringList vheaders;
    vheaders.push_back(QString::fromUtf8("Count"));
    tw.setVerticalHeaderLabels(vheaders);
    int i = 0;
    QStringList hheaders;
    for (std::vector<size_t>::iterator it = nonzero_begin;
            it != nonzero_end; ++it, ++i) {
        //tw.setItem(0, i, new QTableWidgetItem(QString::number(ofs + i)));
        hheaders.push_back(QString::number(ofs + i));
        tw.setItem(0, i, new QTableWidgetItem(QString::number(*it)));
    }
    tw.setHorizontalHeaderLabels(hheaders);
}

template<class MeshT>
void ValenceHistogramDialog::init(MeshT &mesh) {
    /*
     * Vertices
     */
    vertex_valence_hist.clear();
    for (typename MeshT::VertexIter v_it = mesh.vertices_begin(),
            v_end = mesh.vertices_end(); v_it != v_end; ++v_it) {
        size_t valence = mesh.valence(*v_it);
        if (vertex_valence_hist.size() <= valence) {
            vertex_valence_hist.resize(valence + 1, 0);
        }
        vertex_valence_hist[valence] += 1;
    }
    vertexValenceChart_wdgt->setHistogram(&vertex_valence_hist);
    fillHistogramTable(vertex_valence_hist, *vertexValence_tw);

    /*
     * Faces
     */
    face_valence_hist.clear();
    for (typename MeshT::FaceIter f_it = mesh.faces_begin(),
            f_end = mesh.faces_end(); f_it != f_end; ++f_it) {
        size_t valence = mesh.valence(*f_it);
        std::cout << "Face " << f_it->idx() << " has valence " << valence << "." << std::endl;
        if (face_valence_hist.size() <= valence) {
            face_valence_hist.resize(
                    valence + 1, 0);
        }
        face_valence_hist[valence] += 1;
    }

    for (unsigned int i = 0; i < face_valence_hist.size(); ++i) {
        std::cout << "Got " << face_valence_hist[i] << " faces of valence " << i << "." << std::endl;
    }

    faceValenceChart_wdgt->setHistogram(&face_valence_hist);
    fillHistogramTable(face_valence_hist, *faceValence_tw);
}
