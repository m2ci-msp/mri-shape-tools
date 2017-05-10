#ifndef __SPEAKER_SPECIFICITY_MEASURER_H__
#define __SPEAKER_SPECIFICITY_MEASURER_H__

#include "SpecificityMeasurer.h"

class SpeakerSpecificityMeasurer : public SpecificityMeasurer{

public:

  /*-----------------------------------------------------------------------*/

  SpeakerSpecificityMeasurer(const std::vector<Mesh> meshes,
                             const ModelBuilder& builder) :
    SpecificityMeasurer(meshes, builder) {
  }

  /*-----------------------------------------------------------------------*/

  void set_truncated_pose_mode(const int& poseMode) {

    if(this->fixedPose == true) {
      throw std::logic_error("Cannot truncate and fix pose at the same time.");
    }

    this->truncatedPoseMode = poseMode;

    this->truncatedPose = true;

  }

  /*-----------------------------------------------------------------------*/

  void set_fixed_pose(const arma::vec& phonemeWeights) {

    if(this->truncatedPose == true) {
      throw std::logic_error("Cannot truncate and fix pose at the same time.");
    }

    this->fixedPhonemeWeights = phonemeWeights;

    // use original dimension
    this->truncatedPoseMode = phonemeWeights.n_elem;

    this->fixedPose = true;

  }

  /*-----------------------------------------------------------------------*/

protected:

  /*-----------------------------------------------------------------------*/

  virtual Mesh generate_shape() {

    if( this->fixedPose == true ) {
      return
        this->model.sample().generate_with_fixed_phoneme(
          this->fixedPhonemeWeights);
    }
    else{
      return this->model.sample().generate();
    }

  }

  /*-----------------------------------------------------------------------*/

  virtual void init() {

    this->builder.set_truncated_phoneme_mode_dimension(
      this->truncatedPoseMode
      );

  }

  /*-----------------------------------------------------------------------*/

  virtual void create_model(const int& speakerDimension) {

    this->builder.set_truncated_speaker_mode_dimension(speakerDimension);
    this->model = this->builder.build();

  }

  /*-----------------------------------------------------------------------*/

private:

  /*-----------------------------------------------------------------------*/

  int truncatedPoseMode = 1;
  arma::vec fixedPhonemeWeights;

  /*-----------------------------------------------------------------------*/

  bool fixedPose = false;
  bool truncatedPose = false;

  /*-----------------------------------------------------------------------*/

};

#endif
