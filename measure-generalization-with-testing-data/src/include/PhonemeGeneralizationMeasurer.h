#ifndef __PHONEME_GENERALIZATION_MEASURER_H__
#define __PHONEME_GENERALIZATION_MEASURER_H__

#include "GeneralizationMeasurer.h"

class PhonemeGeneralizationMeasurer : public GeneralizationMeasurer{

public:
  PhonemeGeneralizationMeasurer(
                                const SampleDataBase& trainingDataBase,
                                const SampleDataBase& testingDataBase) :
    GeneralizationMeasurer(trainingDataBase, testingDataBase) {

  }

protected:

  /*-----------------------------------------------------------------------*/

  virtual int get_mode_dimension() {

    return this->phonemeDimension;

  }

  /*-----------------------------------------------------------------------*/

  virtual void create_truncated_model(const int& phonemeDimension) {

    this->builder.set_truncated_phoneme_mode_dimension(phonemeDimension);

    if(this->restrictedMode > 0) {

      this->builder.set_truncated_speaker_mode_dimension(this->restrictedMode);

    }

    this->currentModel = builder.build();

  }

  /*-----------------------------------------------------------------------*/

  // parent class members:

  // int restrictedMode;
  // ModelBuilder builder;
  // Model currentModel;
  // int phonemeDimension;

  /*-----------------------------------------------------------------------*/

};

#endif
