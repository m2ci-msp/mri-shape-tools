#ifndef __ENERGY_FUNCTION_H__
#define __ENERGY_FUNCTION_H__

#include <vector>
#include <armadillo>

#include "scan/Scan.h"
#include "alignment/RigidTransformation.h"

class EnergyFunction{

public:
  EnergyFunction(
    const Scan& input
    ) : scan(input) {

  }

  /*----------------------------------------------------------------------*/

  double get_energy(const double& threshold) {

    this->segment = Scan(this->scan);

    segment.transform()->threshold(threshold);

    const double distance = compute_distance();

    return distance;
  }

  /*----------------------------------------------------------------------*/

private:

  /*----------------------------------------------------------------------*/

  void compute_means() {

    this->meanOutside = 0;
    this->meanInside = 0;

    const auto& indicator = this->segment.data()->get_values();
    const auto& values = this->scan.data()->get_values();

    int amountOutside = 0;
    int amountInside = 0;

    for(unsigned int i = 0; i < values.size(); ++i) {
      if( indicator.at(i) == 0 ) {
        ++amountOutside;
        this->meanOutside += values.at(i);

      }
      else {
        ++amountInside;
        this->meanInside += values.at(i);
      }
    }

    this->meanOutside /= amountOutside;
    this->meanInside /= amountInside;

  }

  /*----------------------------------------------------------------------*/

  void compute_stds() {

    this->stdOutside = 0;
    this->stdInside = 0;

    const auto& indicator = this->segment.data()->get_values();
    const auto& values = this->scan.data()->get_values();

    int amountOutside = 0;
    int amountInside = 0;

    double varOutside = 0;
    double varInside = 0;

    for(unsigned int i = 0; i < values.size(); ++i) {
      if( indicator.at(i) == 0 ) {
        ++amountOutside;
        varOutside += pow(values.at(i) - this->meanOutside, 2);
      }
      else {
        ++amountInside;
        varInside += pow(values.at(i) - this->meanInside, 2);
      }
    }

    this->stdOutside = sqrt(varOutside / amountOutside);
    this->stdInside = sqrt(varInside / amountInside);

  }

  /*----------------------------------------------------------------------*/

  double compute_distance() {

    compute_means();
    compute_stds();

    const auto& indicator = this->segment.data()->get_values();
    const auto& values = this->scan.data()->get_values();

    double varOutside = 0;
    double varInside = 0;
    int amountOutside = 0;
    int amountInside = 0;

    // compute variances inside the regions
    for(unsigned int i = 0; i < values.size(); ++i) {
      if( indicator.at(i) == 0 ) {

        const double dist = values.at(i) - this->meanOutside;

        ++amountOutside;
        varOutside += pow(dist, 2);

      }
      else {

        const double dist = values.at(i) - this->meanInside;

        ++amountInside;
        varInside += pow(dist, 2);

      }
    }

    const double varianceEnergy =
      varOutside / amountOutside + varInside / amountInside;

    return  varianceEnergy;

  }

  /*----------------------------------------------------------------------*/

  Scan scan;
  Scan segment;

  // means in the regions
  double meanOutside;
  double meanInside;

  // standard deviations in the regions
  double stdOutside;
  double stdInside;


};

#endif
