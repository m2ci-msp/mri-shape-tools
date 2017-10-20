#ifndef __EMA_COIL_H__
#define __EMA_COIL_H__

class EmaCoil{

public:

  // initialize positional data
  EmaCoil(const std::vector<arma::vec>& coilPositions) :
  
    coilPositions(coilPositions) {
  
  }
  
  // default constructor
  EmaCoil() {
  }

  // return a reference to the positional data vector
  std::vector<arma::vec>& position() {
  
    return this->coilPositions;
  
  }

  // return a const reference to the positional data vector
  const std::vector<arma::vec>& const_position() const {
  
    return this->coilPositions;
  
  }

private:

  // positions of the coil for all time frames
  std::vector<arma::vec> coilPositions;

};
#endif
