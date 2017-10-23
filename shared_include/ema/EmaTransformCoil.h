#ifndef __EMA_TRANSFORM_COIL_H__
#define __EMA_TRANSFORM_COIL_H__

#include <vector>
#include <armadillo>

class EmaTransformCoil{

public:

  EmaTransformCoil( std::vector<arma::vec>& coilPositions ) :
  
  coilPositions(coilPositions) {
  
  }

  void scale(const double& scaling) {
  
    for(arma::vec& position : this->coilPositions ) {
  
      position *= scaling;
  
    }
  
  }

private:

  // reference to the coil positions
  std::vector<arma::vec>& coilPositions;

};
#endif
