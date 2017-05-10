#ifndef __POSE_SPECIFICITY_MEASURER_H__
#define __POSE_SPECIFICITY_MEASURER_H__

#include "SpecificityMeasurer.h"

class PoseSpecificityMeasurer : public SpecificityMeasurer{

public:

  /*-----------------------------------------------------------------------*/

  PoseSpecificityMeasurer(const std::vector<Mesh> meshes,
                          const ModelBuilder& builder) :
    SpecificityMeasurer(meshes, builder) {
  }

  /*-----------------------------------------------------------------------*/

  void set_truncated_speaker_mode(const int& speakerMode) {

    this->truncatedSpeakerMode = speakerMode;

  }

  /*-----------------------------------------------------------------------*/

protected:

  /*-----------------------------------------------------------------------*/

  virtual void init() {

    this->builder.set_truncated_speaker_mode_dimension(
      this->truncatedSpeakerMode
      );

  }

  /*-----------------------------------------------------------------------*/

  virtual void create_model(const int& poseDimension) {

    this->builder.set_truncated_phoneme_mode_dimension(poseDimension);
    this->model = this->builder.build();

  }

  /*-----------------------------------------------------------------------*/

private:

  /*-----------------------------------------------------------------------*/

  int truncatedSpeakerMode = 1;

  /*-----------------------------------------------------------------------*/

};

#endif
