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

/// \file join/label_histogram/histo_join_impl.h
///
/// \details
/// Implements the HJoin tree similarity join.

#ifndef TREE_SIMILARITY_JOIN_HISTOGRAM_HISTO_JOIN_IMPL_H
#define TREE_SIMILARITY_JOIN_HISTOGRAM_HISTO_JOIN_IMPL_H

template <typename Label, typename CostModel, typename VerificationAlgorithm>
HJoin<Label, CostModel, VerificationAlgorithm>::HJoin() {
  pre_candidates_ = 0;
  sum_subproblem_counter_ = 0;
  il_lookups_ = 0;
}

template <typename Label, typename CostModel, typename VerificationAlgorithm>
void HJoin<Label, CostModel, VerificationAlgorithm>::execute_join(
    std::vector<node::Node<Label>>& trees_collection,
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>>& label_histogram_collection,
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>>& degree_histogram_collection,
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>>& leaf_distance_histogram_collection,
    std::vector<std::pair<unsigned int, unsigned int>>& candidates,
    std::vector<join::JoinResultElement>& join_result,
    const double distance_threshold) {

  // Convert trees to leaf distance, label, and degree histograms.
  convert_trees_to_histograms(trees_collection, label_histogram_collection, degree_histogram_collection, leaf_distance_histogram_collection);

  // Retrieves candidates from the candidate index.
  retrieve_candidates(label_histogram_collection, degree_histogram_collection, leaf_distance_histogram_collection, candidates, distance_threshold);

  // Verify all computed join candidates and return the join result.
  verify_candidates(trees_collection, candidates, join_result, distance_threshold);
}


template <typename Label, typename CostModel, typename VerificationAlgorithm>
void HJoin<Label, CostModel, VerificationAlgorithm>::convert_trees_to_histograms(
    std::vector<node::Node<Label>>& trees_collection,
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>>& label_histogram_collection,
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>>& degree_histogram_collection,
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>>& leaf_distance_histogram_collection) {

  // Convert trees to histograms and get the result.
  histogram_converter::Converter<Label> hc;
  hc.create_histogram(trees_collection, label_histogram_collection, degree_histogram_collection, leaf_distance_histogram_collection);
  il_size_ = hc.get_number_of_labels();
}

template <typename Label, typename CostModel, typename VerificationAlgorithm>
void HJoin<Label, CostModel, VerificationAlgorithm>::retrieve_candidates(
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>>& label_histogram_collection,
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>>& degree_histogram_collection,
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>>& leaf_distance_histogram_collection,
    std::vector<std::pair<unsigned int, unsigned int>>& candidates,
    const double distance_threshold) {

  // Initialize candidate index.
  histo_candidate_index::CandidateIndex c_index;

  // Retrieve candidates from the candidate index.
  c_index.lookup(label_histogram_collection, degree_histogram_collection, leaf_distance_histogram_collection, 
      candidates, il_size_, distance_threshold);

  // Copy the number of pre-candidates.
  pre_candidates_ = c_index.get_number_of_pre_candidates();
  // Copy the number of inverted list lookups.
  il_lookups_ = c_index.get_number_of_il_lookups();
}

template <typename Label, typename CostModel, typename VerificationAlgorithm>
void HJoin<Label, CostModel, VerificationAlgorithm>::verify_candidates(
    std::vector<node::Node<Label>>& trees_collection,
    std::vector<std::pair<unsigned int, unsigned int>>& candidates,
    std::vector<join::JoinResultElement>& join_result,
    const double distance_threshold) {

  VerificationAlgorithm ted_algorithm;

  // Verify each pair in the candidate set
  for(const auto& pair: candidates) {
    double ted_value = ted_algorithm.verify(trees_collection[pair.first],
                                            trees_collection[pair.second],
                                            distance_threshold);
    if(ted_value <= distance_threshold)
      join_result.emplace_back(pair.first, pair.second, ted_value);
    
    // Sum up all number of subproblems
    sum_subproblem_counter_ += ted_algorithm.get_subproblem_count();
  }
}

template <typename Label, typename CostModel, typename VerificationAlgorithm>
const unsigned long long int
    HJoin<Label, CostModel, VerificationAlgorithm>::get_number_of_pre_candidates() const {
  return pre_candidates_;
}

template <typename Label, typename CostModel, typename VerificationAlgorithm>
const unsigned long long int HJoin<Label, CostModel, VerificationAlgorithm>::get_subproblem_count() const {
  return sum_subproblem_counter_;
}

template <typename Label, typename CostModel, typename VerificationAlgorithm>
const unsigned long long int
    HJoin<Label, CostModel, VerificationAlgorithm>::get_number_of_il_lookups() const {
  return il_lookups_;
}

#endif // TREE_SIMILARITY_JOIN_HISTOGRAM_HISTO_JOIN_IMPL_H
