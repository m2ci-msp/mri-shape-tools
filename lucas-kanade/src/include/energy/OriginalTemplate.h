#ifndef __ORIGINAL_TEMPLATE_H__
#define __ORIGINAL_TEMPLATE_H__

#include <vector>

#include <armadillo>

#include "image/Image.h"

namespace lucasKanade{

  class OriginalTemplate{

  private:

    const std::vector<arma::vec> originalLocations;

    std::vector<arma::vec> locations;

    std::vector<bool> locationValid;

    Image image;

    double mean;
    double norm;

    std::vector<double> values;

    std::vector<double> centeredValues;

    std::vector<double> normalizedValues;

  public:

    /*--------------------------------------------------------------------------*/

    OriginalTemplate(
                     const std::vector<arma::vec>& originalLocations,
                     const Image& image
                     ) :
      originalLocations(originalLocations), image(image) {

    }

    /*--------------------------------------------------------------------------*/

    void compute(const std::vector<bool>& locationValid) {

      this->locationValid = locationValid;

      assemble_locations();

      compute_values();

      compute_mean();

      compute_centered_values();

      compute_norm();

      compute_normalized_values();

    }


    /*--------------------------------------------------------------------------*/

    const std::vector<double>& get_normalized_values() const {

      return this->normalizedValues;

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    void assemble_locations() {

      this->locations.clear();

      for(size_t i = 0; i < this->originalLocations.size(); ++i) {

        if(this->locationValid[i] == true) {

          this->locations.push_back(this->originalLocations[i]);

        }

      }

    }

    /*--------------------------------------------------------------------------*/

    void compute_values() {

      this->values.clear();

      for(const arma::vec& location : this->locations ) {

        const double& value = this->image.access().at_coordinate(location(0), location(1), location(2));
        this->values.push_back(value);

      }


    }

    /*--------------------------------------------------------------------------*/

    void compute_mean() {

      this->mean = 0;

      for(const double& value: this->values) {

        this->mean += value;

      }

      this->mean /= this->values.size();

    }

    /*--------------------------------------------------------------------------*/

    void compute_centered_values() {

      this->centeredValues.clear();

      for(const double& value: this->values) {

        this->centeredValues.push_back(value - this->mean);

      }

    }

    /*--------------------------------------------------------------------------*/

    void compute_norm() {

      double tmp = 0;

      for(const double& value: this->centeredValues) {

        tmp += pow(value, 2);

      }

      this->norm = sqrt(tmp);

    }

    /*--------------------------------------------------------------------------*/

    void compute_normalized_values() {

      this->normalizedValues.clear();

      for(const double& value: this->centeredValues) {

        this->normalizedValues.push_back(value / this->norm);

      }

    }

    /*--------------------------------------------------------------------------*/

  };

}
#endif
