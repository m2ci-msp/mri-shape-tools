#ifndef __CORRESPONDENCE_SUBSET_FINDER_H__
#define __CORRESPONDENCE_SUBSET_FINDER_H__

#include <vector>
#include <utility>
#include <set>

#include <armadillo>

#include "optimization/rigidalignment/Energy.h"
#include "optimization/rigidalignment/EnergyData.h"
#include "optimization/rigidalignment/EnergyMinimizer.h"


#include "mesh/Mesh.h"

class CorrespondenceSubsetFinder{

public:

  CorrespondenceSubsetFinder(
                             const std::vector<arma::vec>& emaData,
                             const rigidAlignment::EnergySettings& energySettings,
                             const rigidAlignment::MinimizerSettings& minimizerSettings,
                             const Mesh& mesh,
                             const std::vector< std::vector<unsigned int> >& subsets,
                             const int& partitionIndex,
                             const int& partitionAmount
                             ) :
    emaData(emaData),
    energySettings(energySettings),
    minimizerSettings(minimizerSettings),
    mesh(mesh),
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

      // check if we have a valid combination
      if( is_valid(combination) == true) {

        fit_combination(combination);

      }
  
    }

    // translate combination entries to actual vertex indices
    convert_to_vertex_indices(this->bestCombination);
  
    this->resultComputed = true;
  
    return *this;
  
  }

  bool is_valid(const std::vector<unsigned int>& combination) const {

    // a combination is invalid if one vertex is ocurring multiple times
    std::set<int> vertices(combination.begin(), combination.end());

    return vertices.size() == combination.size();

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

  std::vector<arma::vec> get_vertices(std::vector<unsigned int> combination) {

    // first convert combination to vertex indices
    convert_to_vertex_indices(combination);

    std::vector<arma::vec> result;

    for(unsigned int& index: combination) {

      result.push_back( this->mesh.get_vertices().at(index) );

    }

    return result;

  }

  // convert subset indices to vertex indices that are stored at the respective indices
  void convert_to_vertex_indices(std::vector<unsigned int>& combination) const {
  
    for(size_t i = 0; i < combination.size(); ++i) {
  
      const unsigned int& subsetIndex = combination.at(i);
  
      combination[i] = this->subsets.at(i).at(subsetIndex);
  
    }
  
  }

  void fit_combination(const std::vector<unsigned int>& combination) {
  
    Mesh source;
    Mesh target;

    source.set_vertices(this->emaData);
    target.set_vertices(get_vertices(combination));

    rigidAlignment::EnergyData data(source, target);

    rigidAlignment::Energy energy(data, this->energySettings);

    rigidAlignment::EnergyMinimizer minimizer(energy, this->minimizerSettings);

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
  rigidAlignment::EnergySettings energySettings;
  
  // settings for the minimizer
  rigidAlignment::MinimizerSettings minimizerSettings;

  const Mesh mesh;
  
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
