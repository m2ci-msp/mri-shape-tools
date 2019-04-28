#ifndef __REGISTERED_EMA_BOUNDARY_H__
#define __REGISTERED_EMA_BOUNDARY_H__

#include "RegisteredEmaData.h"
#include "RegisteredEmaAccess.h"

class RegisteredEmaBoundary{

private:

  RegisteredEmaData& data;

  RegisteredEmaAccess access;

public:

  RegisteredEmaBoundary(RegisteredEmaData& data) :
    data(data),
    access(data) {

  }

  void change_size(const int& size) {

    // create copy of original data
    RegisteredEmaData originalData = this->data;

    RegisteredEmaAccess originalAccess(originalData);

    const int sampleAmount = this->data.timeStamps.size();

    // update boundary size
    this->data.boundarySize = size;

    // resize data storages
    this->data.speakerWeights.resize(sampleAmount + 2 * size);
    this->data.phonemeWeights.resize(sampleAmount + 2 * size);
    this->data.speakerVariations.resize(sampleAmount + 2 * size);
    this->data.phonemeVariations.resize(sampleAmount + 2 * size);
    this->data.targetPoints.resize(sampleAmount + 2 * size);

    // copy original data
    for(int i = 0; i < sampleAmount; ++i) {

      this->access.speaker_weight(i) = originalAccess.speaker_weight(i);
      this->access.phoneme_weight(i) = originalAccess.phoneme_weight(i);
      this->access.speaker_variation(i) = originalAccess.speaker_variation(i);
      this->access.phoneme_variation(i) = originalAccess.phoneme_variation(i);
      this->access.target_points(i) = originalAccess.target_points(i);

    }

  }

};
#endif
