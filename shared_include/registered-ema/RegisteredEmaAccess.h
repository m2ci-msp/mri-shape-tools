#ifndef __REGISTERED_EMA_ACCESS_H__
#define __REGISTERED_EMA_ACCESS_H__

#include "RegisteredEmaData.h"

class RegisteredEmaAccess{

private:

  RegisteredEmaData& data;

public:

  RegisteredEmaAccess(RegisteredEmaData& data) : data(data) {

  }

  arma::vec& speaker_weight(const int& index) {

    // consider boundary
    const int& boundarySize = this->data.boundarySize;

    return this->data.speakerWeights.at(boundarySize + index);

  }

  arma::vec& phoneme_weight(const int& index) {

    // consider boundary
    const int& boundarySize = this->data.boundarySize;

    return this->data.phonemeWeights.at(boundarySize + index);

  }

  std::vector<arma::vec>& target_points(const int& index) {

    // consider boundary
    const int& boundarySize = this->data.boundarySize;

    return this->data.targetPoints.at(boundarySize + index);

  }

  // const versions
  const arma::vec& speaker_weight(const int& index) const {

    // consider boundary
    const int& boundarySize = this->data.boundarySize;

    return this->data.speakerWeights.at(boundarySize + index);

  }

  const arma::vec& phoneme_weight(const int& index) const {

    // consider boundary
    const int& boundarySize = this->data.boundarySize;

    return this->data.phonemeWeights.at(boundarySize + index);

  }

  const std::vector<arma::vec>& target_points(const int& index) const {

    // consider boundary
    const int& boundarySize = this->data.boundarySize;

    return this->data.targetPoints.at(boundarySize + index);

  }

};
#endif
