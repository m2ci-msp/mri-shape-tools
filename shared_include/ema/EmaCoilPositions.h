#ifndef __EMA_COIL_POSITIONS_H__
#define __EMA_COIL_POSITIONS_H__
#include <vector>
#include <armadillo>

class EmaCoilPositions{

public:

  EmaCoilPositions(
             std::vector<arma::vec>& positions
             ) :
  
    positions(positions) {
  
    }

  std::vector<arma::vec>& all() {
  
    return this->positions;
  
  }

  arma::vec& at_index(const int& index) {
  
    return this->positions.at(index);
  
  }

  

private:

  // individual positions of the coil
  std::vector<arma::vec> positions;
  
  // reference to the timeStamps member in the EmaData class
  // used for interpolating positions
  //const std::vector<double>& timeStamps;

};
#endif
