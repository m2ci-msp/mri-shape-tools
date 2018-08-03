#ifndef __SPEAKER_GENERALIZATION_MEASURER_H__
#define __SPEAKER_GENERALIZATION_MEASURER_H__

#include "GeneralizationMeasurer.h"

class SpeakerGeneralizationMeasurer : public GeneralizationMeasurer{

public:
  SpeakerGeneralizationMeasurer(
                                const SampleDataBase& trainingDataBase,
                                const SampleDataBase& testingDataBase) :
    GeneralizationMeasurer(trainingDataBase, testingDataBase) {

  }


protected:

  /*-----------------------------------------------------------------------*/

  virtual int get_mode_dimension() {

    return this->speakerDimension;

  }

  /*-----------------------------------------------------------------------*/

  virtual void create_truncated_model(const int& speakerDimension) {

    this->builder.set_truncated_speaker_mode_dimension(speakerDimension);

    if(this->restrictedMode > 0) {

      this->builder.set_truncated_phoneme_mode_dimension(this->restrictedMode);

    }

    this->currentModel = builder.build();

  }

  /*-----------------------------------------------------------------------*/

  // parent class members:

  // int restrictedMode;
  // ModelBuilder builder;
  // Model currentModel;
  // int speakerDimension;

  /*-----------------------------------------------------------------------*/

};

#endif
