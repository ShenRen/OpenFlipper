/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *                                                                           *
\*===========================================================================*/

// Author: Martin Heistermann, <martin.heistermann()rwth-aachen.de>

#ifndef ACG_HISTOGRAM_HH
#define ACG_HISTOGRAM_HH

#include <vector>
#include <cassert>
#include <memory>
#include <exception>
#include <algorithm>
#include <type_traits>

#include <QString>

#include "../Config/ACGDefines.hh"

namespace ACG {
class ACGDLLEXPORT Histogram {
public:
    enum class LabelType {
        PerBin,
        PerBoundary,
    };

    virtual ~Histogram() = default;
    const std::vector<size_t> &getBins() const { return bins_; }
    const std::vector<double> &getBinWidths() const { return bin_widths_; }
    virtual double getTotalWidth() const = 0;

    virtual LabelType getLabelType() const = 0;
    virtual QString getBoundaryLabel(size_t /*idx*/) const { assert(false); return QString();}
    virtual QString getBinLabel     (size_t /*idx*/) const { assert(false); return QString();}

protected:
    std::vector<size_t> bins_;
    std::vector<double> bin_widths_;
};


// we need to be careful with ranges, some sums (e.g. INT_MAX - INT_MIN) do not fit into a signed int,
// so we store bin sizes as doubles. With specialization or some tricks we
// could probably use the next-biggest integer type, but if we're using
// the biggest integer type already, we should to fall back to double anyways.

template<typename T>
class HistogramT : public Histogram {
public:
    HistogramT(const std::vector<int> &histogram,
               const std::vector<T> &bin_boundaries,
               const std::vector<double> &bin_widths)
    {
        if (bins_.size() != bin_widths_.size()
            || bins_.size() + 1 != bin_boundaries_.size()) {
            throw std::runtime_error("Histogram constructor sizes don't match.");
        }
        bins_ = histogram;
        bin_boundaries_ = bin_boundaries;
        bin_widths_ = bin_widths;
        double range = bin_boundaries.back() - bin_boundaries.front();
        avg_bin_size_ = range / bins_.size();
    }

    template<typename IterT>
    HistogramT(IterT begin, IterT end, size_t max_bins)
    {
        static_assert(std::is_assignable<T&, typename IterT::value_type>::value, "IterT incompatible with T.");
        static_assert(std::is_floating_point<typename IterT::value_type>::value, "HistogramT currently only supports floating point values.");
        assert(max_bins > 0);
        const size_t n = std::distance(begin, end);
        if (n == 0) return;

        const auto minmax = std::minmax_element(begin, end);
        const T min = *minmax.first;
        const T max = *minmax.second;
        const double min_dbl = static_cast<double>(min);
        const double range = static_cast<double>(max) - min_dbl;

        const size_t n_bins_max = std::min(max_bins, n);
        bin_boundaries_.reserve(n_bins_max + 1);

        T last_boundary = min;
        bin_boundaries_.push_back(min);
        for (size_t i = 1; i < n_bins_max; ++i) {
            // Adding range/n_bins to a accumulator might seem more efficient/elegant,
            // but might cause numeric issues.

            // This multiplication order is bad for huge ranges that cause overflows,
            // however I assume tiny ranges are more common than huge values and more
            // important to get right. If you disagree, add a case distinction or something better.

            T boundary = static_cast<T>(min + (i * range) / n_bins_max);
            // avoid zero-sized bins (happens for many ints with values in a small range)
            if (boundary != last_boundary || i == 0) {
                bin_boundaries_.push_back(boundary);
                bin_widths_.push_back(boundary - last_boundary);
            }
            last_boundary = boundary;
        }
        bin_boundaries_.push_back(max); // avoid rounding issues etc by explicitly picking max.
        bin_widths_.push_back(max - last_boundary);

        bin_boundaries_.shrink_to_fit();
        size_t n_bins = bin_boundaries_.size() - 1;
        bins_.resize(n_bins);

        // note that due to rounding, our bins may have differing sizes, which matters
        // if we handle integral types (relative size difference worst case: bin width 1 vs 2).
        // Be careful to select the right bin.
        std::for_each(begin, end, [&](const T &val) {
            auto it = std::upper_bound(bin_boundaries_.begin(), bin_boundaries_.end(), val);
            if (it == bin_boundaries_.end()) --it; // the last value is exactly max!
            size_t idx = std::distance(bin_boundaries_.begin(), it);
            assert(idx > 0);
            ++bins_[idx - 1];
        });
        avg_bin_size_ = range / n_bins;
    }

    const std::vector<T> &getBinBoundaries() const {
        return bin_boundaries_;
    }

    double getTotalWidth() const override
    {
        return bin_boundaries_.back() - bin_boundaries_.front();
    }

    LabelType getLabelType() const override
    {
        return LabelType::PerBoundary;
    }

    QString getBoundaryLabel (size_t idx) const override;


private:
    std::vector<T> bin_boundaries_;
    double avg_bin_size_ = 0.0;
};


template<typename T>
QString HistogramT<T>::getBoundaryLabel(size_t idx) const {
    return QString::number(bin_boundaries_[idx]);
}

} // namespace ACG

#endif // ACG_HISTOGRAM_HH
