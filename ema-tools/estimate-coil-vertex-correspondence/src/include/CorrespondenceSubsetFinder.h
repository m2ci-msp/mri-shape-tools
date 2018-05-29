#ifndef __CORRESPONDENCE_SUBSET_FINDER_H__
#define __CORRESPONDENCE_SUBSET_FINDER_H__

#include <vector>
#include <utility>
#include <armadillo>

#include "optimization/fitmodel/Energy.h"
#include "optimization/fitmodel/EnergyMinimizer.h"
#include "optimization/fitmodel/EnergySettings.h"
#include "optimization/fitmodel/MinimizerSettings.h"

#include "model/Model.h"

class CorrespondenceSubsetFinder{

public:

  CorrespondenceSubsetFinder(
                             const std::vector<arma::vec>& emaData,
                             const fitModel::EnergySettings& energySettings,
                             const fitModel::MinimizerSettings& minimizerSettings,
                             const Model& model,
                             const std::vector< std::vector<unsigned int> >& subsets,
                             const int& partitionIndex,
                             const int& partitionAmount
                             ) :
    emaData(emaData),
    energySettings(energySettings),
    minimizerSettings(minimizerSettings),
    model(model),
    subsets(subsets),
    partitionIndex(partitionIndex),
    partitionAmount(partitionAmount) {
  
    compute_combination_amount();
    verify_partition();
    compute_partition_bounds();
    derive_subset_factors();
  
  }

  CorrespondenceSubsetFinder& find_best_correspondence() {
  
    for(int i = this->partitionStart; i < this->partitionEnd; ++i) {
  
      std::vector<unsigned int> combination = derive_combination(i);
      fit_combination(combination);
  
    }
  
    // translate combination entries to actual vertex indices
    convert_to_vertex_indices(this->bestCombination);
  
    this->resultComputed = true;
  
    return *this;
  
  }

  

  CorrespondenceSubsetFinder& get_best_correspondence(std::vector<unsigned int>& correspondence) {
  
    if( this->resultComputed == false) {

      throw std::runtime_error("Best correspondence was not computed.");

    }

    correspondence = this->bestCombination;
    return *this;
  
  }
  
  CorrespondenceSubsetFinder& get_best_energy(double& energy) {

    if( this->resultComputed == false) {

      throw std::runtime_error("Best correspondence was not computed.");

    }
    energy = this->bestEnergy;
    return *this;
  
  }

private:

  void compute_combination_amount() {
  
    this->combinationAmount = 1;
  
    for(const auto& subset: this->subsets) {
  
      this->combinationAmount *= subset.size();
  
    }
  
  }

  void verify_partition() const {
  
    if(this->partitionIndex < 1) {
  
      throw std::runtime_error("Index smaller 1 not allowed.");
  
    }
  
    if(this->partitionIndex > this->partitionAmount) {
  
      throw std::runtime_error("Index larger than partition amount.");
  
    }
  
  }

  void compute_partition_bounds() {
  
    const double partitionSize = (double) this->combinationAmount / this->partitionAmount;
  
    // compute start
    this->partitionStart = (this->partitionIndex - 1) * partitionSize;
    this->partitionEnd = ( this->partitionIndex == this->partitionAmount )? combinationAmount: this->partitionStart + partitionSize;
  
  }

  /*
    This methods computes the factors that are used in the 'derive_combination' method for deriving
    the subset indices from a given number.
  */
  void derive_subset_factors() {

    int initial = this->combinationAmount;
  
    for(const auto& subset: this->subsets) {
  
      initial /= subset.size();
  
      this->subsetFactors.push_back(initial);
  
    }
  
  }

  /*
    This is a method for computing the subset indices participating in a combination that is
    represented by a number.

    We have $ \Pi_{i=0}^k \|M_i\|$ combinations where $\|M_i\|$ is the cardinality of
    subset $M_i$ and $k$ is the amount of subsets.

    Let $k$ be $3$ and $a, b, c$ the cardinalities of the subsets.

    Given a number $z = i b c + j c + k$, we can compute $i < a, j < b$, and $k < c$ as follows:

    \begin{align}
    i &= z / ( b  c ) \\
    j &= z mod ( b c ) / c \\
    k &= z mod ( c )
    \end{align}

    i, j, and k are the indices of the individual subsets.

  */
  std::vector<unsigned int> derive_combination(int number) const {
  
    std::vector<unsigned int> result;
  
    for(size_t i = 0; i < this->emaData.size(); ++i) {
  
      result.push_back( number / this->subsetFactors[i] );
  
      number %= this->subsetFactors[i];
  
    }
  
    return result;
  
  }


  /* method for reordering the target points to preserve the correspondence with vertices that
     have been ordered
  */
  std::vector<arma::vec> get_target_points_for_combination(std::vector<unsigned int> combination) {
  
    // convert to vertex indices
    convert_to_vertex_indices(combination);
  
    // create correspondences between vertex index and target point
    std::vector< std::pair<unsigned int, arma::vec> > correspondences;
  
    for(size_t i = 0; i < combination.size(); ++i) {
  
      correspondences.push_back( std::make_pair( combination.at(i), this->emaData.at(i) ) );
  
    }
  
    // compare data structure for ordering
    struct {
  
      bool operator() (
                       const std::pair<unsigned int, arma::vec>& first,
                       const std::pair<unsigned int, arma::vec>& second) const {
  
        return first.first < second.first;
  
      }
  
    } compare;
  
    // sort correspondences
    std::sort( correspondences.begin(), correspondences.end(), compare);
  
    // create reordered target points
    std::vector<arma::vec> result;
  
    for(const auto& correspondence: correspondences) {
  
      result.push_back(correspondence.second);
  
    }
  
    return result;
  
  }

  Model create_truncated_model(std::vector<unsigned int> combination) {
  
    // first convert combination to vertex indices
    convert_to_vertex_indices(combination);
  
    // convert to vertex coordinate indices
    convert_to_coordinate_indices(combination);
  
    // now truncate model
    // first create copy
    Model result = this->model;
  
    std::set<int> vertices(combination.begin(), combination.end());
  
    result.truncate().vertex(vertices);
  
    return result;
  
  }

  // convert subset indices to vertex indices that are stored at the respective indices
  void convert_to_vertex_indices(std::vector<unsigned int>& combination) const {
  
    for(size_t i = 0; i < combination.size(); ++i) {
  
      const unsigned int& subsetIndex = combination.at(i);
  
      combination[i] = this->subsets.at(i).at(subsetIndex);
  
    }
  
  }

  // convert to coordinate indices belonging to the vertex indices
  void convert_to_coordinate_indices(std::vector<unsigned int>& combination) {
  
    std::vector<unsigned int> result;
  
    for(const unsigned int& entry: combination) {
  
      // each vertex has three entries:
  
      // one for x
      result.push_back(3 * entry + 0);
      // one for y
      result.push_back(3 * entry + 1);
      // and one for z
      result.push_back(3 * entry + 2);
  
    }
  
    combination = result;
  
  }

  void fit_combination(const std::vector<unsigned int>& combination) {
  
    // truncate model to used combination
    Model model = create_truncated_model(combination);
  
    // create energy data
  
    // first create mesh
    Mesh mesh;
  
    // we might have to rearrange the EMA data
    // this is due to how the truncation is performed:
    // the vertices are ordered
    std::vector<arma::vec> targetPoints = get_target_points_for_combination(combination);
  
    mesh.set_vertices(targetPoints);
    fitModel::EnergyData energyData(model, mesh);
  
    // create energy
    fitModel::Energy energy(energyData, this->energySettings);
  
    // create minimizer
    fitModel::EnergyMinimizer minimizer(energy, this->minimizerSettings);
  
    minimizer.minimize();
  
    // get end energy
    const double endEnergy = energy.state().energy;
  
    if( endEnergy < this->bestEnergy ) {
  
      this->bestEnergy = endEnergy;
      this->bestCombination = combination;
  
    }
  
  }

  // energy for best combination
  double bestEnergy = DBL_MAX;
  
  // currently best combination
  std::vector<unsigned int> bestCombination;
  
  // target coil positions
  const std::vector<arma::vec> emaData;
  
  // energy settings
  fitModel::EnergySettings energySettings;
  
  // settings for the minimizer
  fitModel::MinimizerSettings minimizerSettings;
  
  // base multilinear model
  const Model model;
  
  // vertex indices of all subsets
  std::vector< std::vector<unsigned int> > subsets;
  
  // factors for deriving the combination
  std::vector<int> subsetFactors;
  
  int partitionStart;
  int partitionEnd;
  
  const int partitionIndex = 1;
  const int partitionAmount = 1;
  
  int combinationAmount;
  
  bool resultComputed = false;

};

#endif
