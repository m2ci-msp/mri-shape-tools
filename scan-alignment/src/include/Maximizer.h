#ifndef __MAXIMIZER_H__
#define __MAXIMIZER_H__

#include <vector>
#include <dlib/optimization.h>
#include <armadillo>

#include "alignment/RigidTransformation.h"
#include "EnergyFunction.h"

typedef dlib::matrix<double,0,1> column_vector;

// Wrapper class for dlib
class MaximizerGetEnergy {

  public:

    MaximizerGetEnergy (EnergyFunction& function) :
      function(function) {
      }

    double operator() ( const column_vector& x ) const
    {
      arma::vec translation({x(0), x(1), x(2)});
      arma::vec rotation({x(3), x(4), x(5)});
      return this->function.get_energy(translation, rotation);
    }

  private:

    EnergyFunction& function;
};

/* Maximizes the given energy by using a quasi-Newton approach */
class Maximizer{

  public:
    Maximizer(EnergyFunction& function):
      get_energy(MaximizerGetEnergy(function)) {

        this->delta = 10e-9;

      }

    /*-----------------------------------------------------------------------*/

    RigidTransformation get_maximizer(
        const Mesh& sourceMesh,
        std::vector<double>& start,
        const std::vector<double>& lower,
        const std::vector<double>& upper
        ) {

      const size_t length = start.size();

      column_vector x(length);
      column_vector lowerBound(length);
      column_vector upperBound(length);

      for(size_t i = 0; i < length; ++i) {
        x(i) = start.at(i);
        lowerBound(i) = lower.at(i);
        upperBound(i) = upper.at(i);
      }

      // maximize the normalized cross-correlation
      dlib::find_max_box_constrained(
          dlib::lbfgs_search_strategy(start.size()),
          dlib::objective_delta_stop_strategy(this->delta).be_verbose(),
          this->get_energy, dlib::derivative(this->get_energy, 1), x,
          lowerBound,
          upperBound
          );

      for(size_t i = 0; i < start.size(); ++i) {
        start[i] = x(i);
      }

      // construct the rigid transformation from the found maximizer
      arma::vec t({start.at(0), start.at(1), start.at(2)});

      RigidTransformation transformation;

      transformation.set_origin(sourceMesh.get_center());
      transformation.set_translation(t);
      transformation.set_rotation(start.at(3), start.at(4), start.at(5));

      return transformation;


    }

    /*-----------------------------------------------------------------------*/

    void set_delta(const double& value) {
      this->delta = value;
    }

    /*-----------------------------------------------------------------------*/


  private:

    /*-----------------------------------------------------------------------*/

    MaximizerGetEnergy get_energy;

    /*-----------------------------------------------------------------------*/

    double delta;

};

#endif
