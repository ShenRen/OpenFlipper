/*
 * ValenceHistogramDialog.cc
 *
 *  Created on: Jan 27, 2016
 *      Author: hc
 */

#include "ValenceHistogramDialog.hh"

#include <ACG/Utils/SmartPointer.hh>

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

namespace {

class ValenceHistogram : public ACG::Histogram {
public:
    explicit ValenceHistogram(const std::vector<size_t> &bins)
    {
        std::vector<size_t>::const_iterator nonzero_begin = bins.begin();
        for (; nonzero_begin != bins.end() && *nonzero_begin == 0;
                ++nonzero_begin);
        if (nonzero_begin == bins.end()) return;

        std::vector<size_t>::const_iterator nonzero_end = bins.end();
        for (; (nonzero_end-1) != nonzero_begin && *(nonzero_end-1) == 0;
                --nonzero_end);

        ofs_ = std::distance(bins.begin(), nonzero_begin);
        bins_.assign(nonzero_begin, nonzero_end);
        bin_widths_.assign(bins_.size() + 1, 1);
    }

    LabelType getLabelType() const override
    {
        return LabelType::PerBin;
    }

    QString getBinLabel (size_t idx) const override
    {
        return QString::number(ofs_ + idx);
    }

    double getTotalWidth() const override {
        return bins_.size();
    }
private:
    size_t ofs_ = 0;
};

} // namespace

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
    vertexValenceChart_wdgt->setHistogram(ptr::make_unique<ValenceHistogram>(vertex_valence_hist));
    fillHistogramTable(vertex_valence_hist, *vertexValence_tw);

    /*
     * Faces
     */
    face_valence_hist.clear();
    for (typename MeshT::FaceIter f_it = mesh.faces_begin(),
            f_end = mesh.faces_end(); f_it != f_end; ++f_it) {
        size_t valence = mesh.valence(*f_it);
        if (face_valence_hist.size() <= valence) {
            face_valence_hist.resize(
                    valence + 1, 0);
        }
        face_valence_hist[valence] += 1;
    }

    faceValenceChart_wdgt->setHistogram(ptr::make_unique<ValenceHistogram>(face_valence_hist));
    fillHistogramTable(face_valence_hist, *faceValence_tw);
}
