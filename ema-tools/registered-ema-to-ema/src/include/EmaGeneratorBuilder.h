#ifndef __EMA_GENERATOR_BUILDER_H__
#define __EMA_GENERATOR_BUILDER_H__

#include <string>

#include "ema/Ema.h"
#include "model/Model.h"
#include "registered-ema/RegisteredEma.h"
#include "EmaGenerator.h"

class EmaGeneratorBuilder{

private:

  Ema headMotion;

  RegisteredEma registeredEma;

  Json::Value globalTransformation;

  double startTime;
  double endTime;

  double samplingRate;

  std::vector<std::string> coilIds;

  bool headMotionSet = false;
  bool registeredEmaSet = false;
  bool globalTransformationSet = false;
  bool startTimeSet = false;
  bool endTimeSet = false;
  bool samplingRateSet = false;
  bool coilIdsSet = false;

public:

  EmaGenerator build() {

    if( is_valid() == false) {

      throw std::logic_error("Not all parameters are set.");

    }

    EmaGenerator generator = EmaGenerator(
                        this->headMotion,
                        this->registeredEma,
                        this->globalTransformation,
                        this->startTime,
                        this->endTime,
                        this->samplingRate,
                        this->coilIds
                        );

    return generator;

  }

  // setters
  EmaGeneratorBuilder& set_head_motion(const Ema& headMotion) {

    this->headMotion = headMotion;
    this->headMotionSet = true;

    return *this;

  }

  EmaGeneratorBuilder& set_registered_ema(const RegisteredEma& registeredEma) {

    this->registeredEma = registeredEma;
    this->registeredEmaSet = true;

    return *this;

  }

  EmaGeneratorBuilder& set_global_transformation(const Json::Value& globalTransformation) {

    this->globalTransformation = globalTransformation;
    this->globalTransformationSet = true;

    return *this;

  }

  EmaGeneratorBuilder& set_start_time(const double& startTime) {

    this->startTime = startTime;
    this->startTimeSet = true;

    return *this;

  }

  EmaGeneratorBuilder& set_end_time(const double& endTime) {

    this->endTime = endTime;
    this->endTimeSet = true;

    return *this;

  }

  EmaGeneratorBuilder& set_sampling_rate(const double& samplingRate) {

    this->samplingRate = samplingRate;
    this->samplingRateSet = true;

    return *this;

  }

  EmaGeneratorBuilder& set_coil_ids(const std::vector<std::string>& coilIds) {

    this->coilIds = coilIds;
    this->coilIdsSet = true;

    return *this;

  }

private:

  bool is_valid() const {

    return (

            headMotionSet &&
            registeredEmaSet &&
            globalTransformationSet &&
            startTimeSet &&
            endTimeSet &&
            samplingRateSet &&
            coilIdsSet

            );

  }

};
#endif
