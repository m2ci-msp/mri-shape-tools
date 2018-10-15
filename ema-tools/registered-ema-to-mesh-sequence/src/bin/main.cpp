#include <fstream>
#include <stdexcept>

#include <vector>
#include <string>
#include <utility>

#include <json/json.h>

#include "registered-ema/RegisteredEma.h"
#include "ema/Ema.h"
#include "model/Model.h"
#include "model/ModelReader.h"

#include "mesh/MeshIO.h"

#include "MeshSequence.h"
#include "MeshSequenceBuilder.h"
#include "MeshSequenceOutput.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Model tongueModel = ModelReader(settings.tongueModel).get_model();
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

  std::vector< std::pair<double, Mesh> > result;

  // initialize basic version
  if(settings.basic == true) {

    MeshSequence sequence(registeredEma, tongueModel, settings.samplingRate);

    if(settings.showCoils == true) {

      Mesh coilMesh = MeshIO::read(settings.coilMesh);
      sequence.set_coil_mesh(coilMesh);

    }

    // set start and/or end time if provided
    if(settings.startTimeSet == true) {

      sequence.set_start_time(settings.startTime);

    }

    if(settings.endTimeSet == true) {

      sequence.set_end_time(settings.endTime);

    }

    result = sequence.build();


  } else {

    // switch to full version if needed
    MeshSequenceBuilder builder;

    builder
      .set_head_motion(headMotion)
      .set_registered_ema(registeredEma)
      .set_tongue_model(tongueModel)
      .set_global_transformation(modifications)
      .set_start_time(settings.startTime)
      .set_end_time(settings.endTime)
      .set_sampling_rate(settings.samplingRate);

    MeshSequence sequence = builder.build();

    if(settings.customTransformationSet == true) {

      sequence.set_custom_transformation(customTransformation);

    }

    if(settings.showCoils == true) {

      Mesh coilMesh = MeshIO::read(settings.coilMesh);
      sequence.set_coil_mesh(coilMesh);

    }

    result = sequence.build();

  }

  MeshSequenceOutput(result, settings.outputBase).output();

  return 0;

}
