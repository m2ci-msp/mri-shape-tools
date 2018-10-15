#ifndef __MESH_SEQUENCE_BUILDER_H__
#define __MESH_SEQUENCE_BUILDER_H__

#include <string>

#include "ema/Ema.h"
#include "model/Model.h"
#include "registered-ema/RegisteredEma.h"
#include "MeshSequence.h"

class MeshSequenceBuilder{

private:

  Ema headMotion;

  RegisteredEma registeredEma;

  Model tongueModel;

  Json::Value globalTransformation;

  double startTime;
  double endTime;

  double samplingRate;

  bool onlyTranslation;

  bool headMotionSet = false;
  bool registeredEmaSet = false;
  bool tongueModelSet = false;
  bool globalTransformationSet = false;
  bool startTimeSet = false;
  bool endTimeSet = false;
  bool samplingRateSet = false;
  bool onlyTranslationSet = false;

public:

  MeshSequence build() {

    if( is_valid() == false) {

      throw std::logic_error("Not all parameters are set.");

    }

    return MeshSequence(
                        this->headMotion,
                        this->registeredEma,
                        this->tongueModel,
                        this->globalTransformation,
                        this->startTime,
                        this->endTime,
                        this->samplingRate,
                        this->onlyTranslation
                        );

  }

  // setters
  MeshSequenceBuilder& set_head_motion(const Ema& headMotion) {

    this->headMotion = headMotion;
    this->headMotionSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_registered_ema(const RegisteredEma& registeredEma) {

    this->registeredEma = registeredEma;
    this->registeredEmaSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_tongue_model(const Model& tongueModel) {

    this->tongueModel = tongueModel;
    this->tongueModelSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_global_transformation(const Json::Value& globalTransformation) {

    this->globalTransformation = globalTransformation;
    this->globalTransformationSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_start_time(const double& startTime) {

    this->startTime = startTime;
    this->startTimeSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_end_time(const double& endTime) {

    this->endTime = endTime;
    this->endTimeSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_sampling_rate(const double& samplingRate) {

    this->samplingRate = samplingRate;
    this->samplingRateSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_only_translation(const bool& onlyTranslation) {

    this->onlyTranslation = onlyTranslation;
    this->onlyTranslationSet = true;

    return *this;

  }

private:

  bool is_valid() const {

    return (

            headMotionSet &&
            registeredEmaSet &&
            tongueModelSet &&
            globalTransformationSet &&
            startTimeSet &&
            endTimeSet &&
            samplingRateSet &&
            onlyTranslationSet

            );

  }

};
#endif
