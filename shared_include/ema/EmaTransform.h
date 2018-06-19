#ifndef __EMA_TRANSFORM_H__
#define __EMA_TRANSFORM_H__

#include "ema/EmaData.h"

#include "matrix/Transformation.h"

class EmaTransform{

private:

  EmaData& emaData;

public:

  EmaTransform(EmaData& emaData) : emaData(emaData) {

  }

  void scale(const double& scaling) {

    for(auto& mapObject: this->emaData.emaCoils ) {

      EmaCoil& coil = mapObject.second;

      coil.transform().scale(scaling);

    }

  }

  void translate(const arma::vec& translation) {

    for(auto& mapObject: this->emaData.emaCoils ) {

      EmaCoil& coil = mapObject.second;

      coil.transform().translate(translation);

    }

  }

  void apply_matrix(const arma::mat& matrix) {

    for(auto& mapObject: this->emaData.emaCoils ) {

      EmaCoil& coil = mapObject.second;

      coil.transform().apply_matrix(matrix);

    }

  }

  void apply_rigid_transformation(const Transformation& transformation) {

    for(auto& mapObject: this->emaData.emaCoils ) {

      EmaCoil& coil = mapObject.second;

      coil.transform().apply_rigid_transformation(transformation);

    }

  }

  void project_to_midsagittal() {

    for(auto& mapObject: this->emaData.emaCoils ) {

      EmaCoil& coil = mapObject.second;

      coil.transform().project_to_midsagittal();

    }

  }

  void map_to_coordinate_system(const EmaCoordinateSystem& system) {

    for(auto& mapObject: this->emaData.emaCoils ) {

      EmaCoil& coil = mapObject.second;

      coil.transform().map_to_coordinate_system(system);

    }

  }



};
#endif
