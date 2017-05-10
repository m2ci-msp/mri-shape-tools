#ifndef __PHONEME_GENERALIZATION_MEASURER_H__
#define __PHONEME_GENERALIZATION_MEASURER_H__

#include "training/SampleDataBase.h"
#include "training/TrainingDataBuilder.h"
#include "training/TensorBuilder.h"

#include "GeneralizationMeasurer.h"

class PhonemeGeneralizationMeasurer : public GeneralizationMeasurer{

public:
  PhonemeGeneralizationMeasurer(const SampleDataBase& database) :
    GeneralizationMeasurer(database) {
  }


protected:

  /*-----------------------------------------------------------------------*/

  virtual void set_mode_ids() {

    this->modeIds = this->database.get_phoneme_ids();

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

  virtual void setup_training_without(const std::string& toBeExcluded) {

    TrainingDataBuilder trainingBuilder(database);

    // remove excluded phoneme from training set
    this->currentTrainingData =
      trainingBuilder.build_without_phoneme(toBeExcluded);

  }

  /*-----------------------------------------------------------------------*/

  virtual void setup_target_with(const std::string& toBeIncluded) {

    // use excluded phoneme as target meshes
    this->currentTargets = this->database.get_phoneme_meshes(toBeIncluded);

  }

  /*-----------------------------------------------------------------------*/


  // parent class members:
  // std::set<std::string> modeIds;
  // const SampleDataBase& database;

  // std::vector<Mesh> currentTargets;
  // TrainingData currentTrainingData;

  /*-----------------------------------------------------------------------*/

};

#endif
