#ifndef __EMA_COIL_TRANSFORM_H__
#define __EMA_COIL_TRANSFORM_H__

#include <armadillo>

#include "ema/EmaCoilData.h"
#include "alignment/RigidTransformation.h"

class EmaCoilTransform{

public:

  // use reference to EmaCoilData to construct object
  EmaCoilTransform( EmaCoilData& emaCoilData ) :
  
  emaCoilData(emaCoilData) {
  
  }

  void scale(const double& scaling) {
  
    for(arma::vec& position : this->emaCoilData.positions) {
  
      position *= scaling;
  
    }
  
  }

  void translate(const arma::vec& translation) {
  
      for(arma::vec& position : this->emaCoilData.positions ) {
  
        position += translation;
  
      }
  
  }

  void apply_matrix(const arma::mat& matrix) {
  
      for(arma::vec& position : this->emaCoilData.positions ) {
  
        position = matrix * position;
  
      }
  
  }

  void apply_rigid_motion(const RigidTransformation& transformation) {
  
      for(arma::vec& position : this->emaCoilData.positions ) {
  
        transformation.apply(position);
  
      }
  
  }

  void project_to_midsagittal() {

    for(arma::vec& position : this->emaCoilData.positions ) {

      // set x-coordinate to 0
      position(0) = 0;

    }

  }

private:

  // reference to the EmaCoilData instance
  EmaCoilData& emaCoilData;

};
#endif
