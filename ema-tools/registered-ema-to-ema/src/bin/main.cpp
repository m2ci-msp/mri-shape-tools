#include <fstream>
#include <stdexcept>

#include <vector>
#include <string>
#include <utility>

#include <json/json.h>

#include "registered-ema/RegisteredEma.h"
#include "ema/Ema.h"

#include "EmaGenerator.h"
#include "EmaGeneratorBuilder.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Ema headMotion;
  Json::Value modifications;
  RegisteredEma registeredEma;

  Json::Value customTransformation;

  registeredEma.read().from(settings.input);

  if(settings.basic == false) {

    headMotion.read().from(settings.headMotionEma);

    std::ifstream modFile(settings.globalTransformation);

    // throw exception if file can not be opened
    if( modFile.is_open() == false ) {

      throw std::runtime_error("Cannot open global transformation file.");

    }

    modFile >> modifications;
    modFile.close();

  }

  if( settings.customTransformationSet == true) {

    std::ifstream modFile(settings.customTransformation);

    // throw exception if file can not be opened
    if( modFile.is_open() == false ) {

      throw std::runtime_error("Cannot open custom transformation file.");

    }

    modFile >> customTransformation;
    modFile.close();

  }

  Ema result;

  // initialize basic version
  if(settings.basic == true) {

    EmaGenerator emaGenerator(registeredEma, settings.samplingRate, settings.coilIds);

    // set start and/or end time if provided
    if(settings.startTimeSet == true) {

      emaGenerator.set_start_time(settings.startTime);

    }

    if(settings.endTimeSet == true) {

      emaGenerator.set_end_time(settings.endTime);

    }

    result = emaGenerator.build();


  } else {

    // switch to full version if needed
    EmaGeneratorBuilder builder;

    builder
      .set_head_motion(headMotion)
      .set_registered_ema(registeredEma)
      .set_global_transformation(modifications)
      .set_start_time(settings.startTime)
      .set_end_time(settings.endTime)
      .set_sampling_rate(settings.samplingRate)
      .set_coil_ids(settings.coilIds);

    EmaGenerator emaGenerator = builder.build();

    if(settings.customTransformationSet == true) {

      emaGenerator.set_custom_transformation(customTransformation);

    }

    result = emaGenerator.build();

  }

  result.write().to(settings.output);

  return 0;

}
