#ifndef __TRAINING_DATA_H__
#define __TRAINING_DATA_H__

#include <vector>
#include <armadillo>

class TrainingData{

public:

  arma::vec mean;
  std::vector<arma::vec> data;
  int speakerAmount;
  int phonemeAmount;
  int spaceSize;

};
#endif
