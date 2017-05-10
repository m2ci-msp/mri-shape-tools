#ifndef __LANDMARK_H__
#define __LANDMARK_H__

#include <armadillo>

struct Landmark{

  Landmark() {
    this->sourceIndex = -1;
    this->sourcePosition = arma::zeros<arma::vec> (3);
    this->targetPosition = arma::zeros<arma::vec> (3);
  }

  Landmark(
      const int& sourceIndex,
      const arma::vec& sourcePosition,
      const arma::vec& targetPosition
      ) :
    sourceIndex(sourceIndex),
    sourcePosition(sourcePosition),
    targetPosition(targetPosition) {
    }

  Landmark(
      const std::string& name,
      const int& sourceIndex,
      const arma::vec& sourcePosition,
      const arma::vec& targetPosition
      ) :
    name(name),
    sourceIndex(sourceIndex),
    sourcePosition(sourcePosition),
    targetPosition(targetPosition) {
    }


  std::string name;
  int sourceIndex;
  arma::vec sourcePosition;
  arma::vec targetPosition;
};
#endif
