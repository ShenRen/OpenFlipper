#include "Histogram.hh"

namespace ACG {

template<>
QString HistogramT<double>::getBoundaryLabel(size_t idx) const {
    // TODO: choose accuracy based on avg_bin_size_
    return QString::number(bin_boundaries_[idx], 'g', 2);
}

} // namespace ACG
