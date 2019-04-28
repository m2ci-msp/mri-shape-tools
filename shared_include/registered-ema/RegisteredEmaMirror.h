#ifndef __REGISTERED_EMA_MIRROR_H__
#define __REGISTERED_EMA_MIRROR_H__

#include "RegisteredEmaData.h"
#include "RegisteredEmaAccess.h"

class RegisteredEmaMirror{

private:

  RegisteredEmaData& data;
  RegisteredEmaAccess access;

public:

  RegisteredEmaMirror(RegisteredEmaData& data) :

    data(data),
    access(data) {

  }

  void speaker_weights() {

    const int sampleAmount = this->data.timeStamps.size();
    const int& boundarySize = this->data.boundarySize;

    for(int i = 0; i < boundarySize; ++i) {

      // left boundary
      this->access.speaker_weight(- i - 1) = this->access.speaker_weight(i);

      // right boundary
      this->access.speaker_weight(sampleAmount + i) = this->access.speaker_weight(sampleAmount - 1 - i);

    }

  }

  void phoneme_weights() {

    const int sampleAmount = this->data.timeStamps.size();
    const int& boundarySize = this->data.boundarySize;

    for(int i = 0; i < boundarySize; ++i) {

      // left boundary
      this->access.phoneme_weight(- i - 1) = this->access.phoneme_weight(i);

      // right boundary
      this->access.phoneme_weight(sampleAmount + i) = this->access.phoneme_weight(sampleAmount - 1 - i);

    }

  }

  void speaker_variations() {

    const int sampleAmount = this->data.timeStamps.size();
    const int& boundarySize = this->data.boundarySize;

    for(int i = 0; i < boundarySize; ++i) {

      // left boundary
      this->access.speaker_variation(- i - 1) = this->access.speaker_variation(i);

      // right boundary
      this->access.speaker_variation(sampleAmount + i) = this->access.speaker_variation(sampleAmount - 1 - i);

    }

  }

  void phoneme_variations() {

    const int sampleAmount = this->data.timeStamps.size();
    const int& boundarySize = this->data.boundarySize;

    for(int i = 0; i < boundarySize; ++i) {

      // left boundary
      this->access.phoneme_variation(- i - 1) = this->access.phoneme_variation(i);

      // right boundary
      this->access.phoneme_variation(sampleAmount + i) = this->access.phoneme_variation(sampleAmount - 1 - i);

    }

  }

  void target_points() {

    const int sampleAmount = this->data.timeStamps.size();
    const int& boundarySize = this->data.boundarySize;

    for(int i = 0; i < boundarySize; ++i) {

      // left boundary
      this->access.target_points(- i - 1) = this->access.target_points(i);

      // right boundary
      this->access.target_points(sampleAmount + i) = this->access.target_points(sampleAmount - 1 - i);

    }

  }

};
#endif
