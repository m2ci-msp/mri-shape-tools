#ifndef __SPECIFICITY_MEASURER_H__
#define __SPECIFICITY_MEASURER_H__

#include <vector>

#include "mesh/Mesh.h"
#include "model/Model.h"
#include "model/ModelBuilder.h"

#include "Subset.h"
#include "ErrorDatabase.h"


class SpecificityMeasurer{

public:

  /*-----------------------------------------------------------------------*/

  SpecificityMeasurer(const std::vector<Mesh>& meshes, const ModelBuilder& builder) :
    meshes(meshes), builder(builder) {
  }

  /*-----------------------------------------------------------------------*/

  SpecificityMeasurer& set_max_components( const int maxComponents) {

    this->maxComponents = maxComponents;

    return *this;

  }

  /*-----------------------------------------------------------------------*/

  SpecificityMeasurer& set_min_components( const int minComponents) {

    this->minComponents = minComponents;

    return *this;

  }

  /*-----------------------------------------------------------------------*/

  SpecificityMeasurer& set_sample_amount( const int sampleAmount) {

    this->sampleAmount = sampleAmount;

    return *this;

  }

  /*-----------------------------------------------------------------------*/

  void measure(std::vector<Subset>& subsets) {

    init();

    for(
      int i_component = maxComponents;
      i_component >= this->minComponents; --i_component) {

      create_model(i_component);

      // create for each subset an error vector
      std::vector<arma::vec> errors;

      for(
        unsigned int i_subset = 0;
        i_subset < subsets.size(); ++i_subset) {

        errors.push_back(arma::zeros(sampleAmount));

      } // end for subsets

#pragma omp parallel for shared(errors)

      for(
        int i_sample = 0;
        i_sample < sampleAmount; ++i_sample) {

        Mesh shape = generate_shape();

        // find for each subset the smallest distance to a mesh
        for(
          unsigned int i_subset = 0;
          i_subset < subsets.size(); ++i_subset) {

          errors.at(i_subset)(i_sample) =
            get_smallest_mean_distance(shape, subsets.at(i_subset).indices);

        } // end for subsets

      } // end for sampleAmount

      // compute statistics for each subset
      for(
        unsigned int i_subset = 0;
        i_subset < subsets.size(); ++i_subset) {

        const double mean = arma::mean(errors.at(i_subset));
        const double standardDeviation = arma::stddev(errors.at(i_subset), 1);

        subsets.at(i_subset).errorDataBase.add_error(
          i_component, mean, standardDeviation
          );

      } // end for subsets


    } // end for components

    return;

  }

  /*-----------------------------------------------------------------------*/

protected:

  /*-----------------------------------------------------------------------*/

  double get_smallest_mean_distance(const Mesh& source,
                                    const std::vector<int>& subset) {
    double minDistance = DBL_MAX;

    for(const Mesh& target: this->meshes) {

      double currentDistance = 0;

      for(const int& index: subset){

        const arma::vec sourcePoint = source.get_vertices().at(index);
        const arma::vec targetPoint = target.get_vertices().at(index);
        currentDistance += arma::norm(targetPoint - sourcePoint);

      } // end subset

      currentDistance /= subset.size();

      minDistance = (currentDistance < minDistance)? currentDistance: minDistance;

    } // end for currentTarget

    return minDistance;

  }

  /*-----------------------------------------------------------------------*/

  virtual Mesh generate_shape() {

    return this->model.sample().generate();

  }


  /*-----------------------------------------------------------------------*/

  virtual void init() = 0;
  virtual void create_model(const int&) = 0;

  /*-----------------------------------------------------------------------*/

  int minComponents = 1;
  int maxComponents = 1;

  int sampleAmount = 1000;

  /*-----------------------------------------------------------------------*/

  std::vector<Mesh> meshes;

  Model model;
  ModelBuilder builder;

  /*-----------------------------------------------------------------------*/

};

#endif
