#ifndef __NORMALIZED_CROSS_CORRELATION_H__
#define __NORMALIZED_CROSS_CORRELATION_H__

#include <vector>
#include <armadillo>

class NormalizedCrossCorrelation{

public:

  /*----------------------------------------------------------------------*/

  static double compute(
    const std::vector<arma::vec>& sourceProfiles,
    const std::vector<arma::vec>& targetProfiles
    ) {

    double distance = 0;

    const size_t size = sourceProfiles.size();

    for(size_t i = 0; i < size; ++i) {

      // center source profile
      arma::vec sourceProfile = center(sourceProfiles.at(i));

      // center target profile
      arma::vec targetProfile = center(targetProfiles.at(i));

      distance += arma::norm_dot(sourceProfile, targetProfile);

    }

    return distance;

  }

  /*----------------------------------------------------------------------*/

  static arma::vec center(const arma::vec& x) {

    arma::vec result = x;
    const double mean = arma::mean(x);

    result.transform( [&](double val) { return (val - mean); } );

    return result;
  }

  /*----------------------------------------------------------------------*/

private:

  /*----------------------------------------------------------------------*/

  NormalizedCrossCorrelation() {
  }

  /*----------------------------------------------------------------------*/

};
#endif
