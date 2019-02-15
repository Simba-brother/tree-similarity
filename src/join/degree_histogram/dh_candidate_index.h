// The MIT License (MIT)
// Copyright (c) 2018 Thomas Huetter
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/// \file join/degree_histogram/dh_candidate_index.h
///
/// \details
/// Implements a candidate index that efficiently and effectively returns tree 
/// pairs that satisfy the degree histogram lower bound by Kailing et al. 

#ifndef TREE_SIMILARITY_JOIN_DEGREE_HISTOGRAM_DH_CANDIDATE_INDEX_H
#define TREE_SIMILARITY_JOIN_DEGREE_HISTOGRAM_DH_CANDIDATE_INDEX_H

#include <cmath>
#include <climits>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "node.h"

namespace dh_candidate_index {

class CandidateIndex {
// Member functions.
public:
  /// Constructor.
  CandidateIndex();
  /// Returns a set of candidates that satisfy the structural filter. 
  ///
  /// \param histogram_collection Collection of all histograms.
  /// \param join_candidates Resultset that contains all join candidates.
  /// \param il_size Size of the inverted list.
  /// \param distance_threshold Given similarity threshold.
  void lookup(
      std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>>& histogram_collection,
      std::vector<std::pair<unsigned int, unsigned int>>& join_candidates,
      const unsigned int il_size,
      const double distance_threshold);
  /// Returns the number of precandidates.
  ///
  /// \return The number of precandidates.
  unsigned long int get_number_of_pre_candidates() const;
  /// Sets the number of precandidates.
  void set_number_of_pre_candidates(const unsigned long int pc);
  /// Returns the number of precandidates.
  ///
  /// \return The number of lookups in the inverted list.
  unsigned long int get_number_of_il_lookups() const;
// Member variables.
private:
  /// Number of precandidates.
  unsigned long int pre_candidates_;
  /// Number of lookups in the inverted list.
  unsigned long int il_lookups_;
};

// Implementation details.
#include "dh_candidate_index_impl.h"

}

#endif // TREE_SIMILARITY_JOIN_DEGREE_HISTOGRAM_DH_CANDIDATE_INDEX_H