#include <fstream>
#include <stdexcept>

#include <vector>
#include <string>
#include <utility>

#include <json/json.h>

#include "RegisteredEma.h"
#include "ema/Ema.h"
#include "model/Model.h"
#include "model/ModelReader.h"

#include "MeshSequence.h"
#include "MeshSequenceBuilder.h"
#include "MeshSequenceOutput.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Model tongueModel = ModelReader(settings.tongueModel).get_model();
  Ema headMotion;
  RegisteredEma registeredEma;

  headMotion.read().from(settings.headMotionEma);
  registeredEma.read().from(settings.input);

  Json::Value modifications;

  std::ifstream modFile(settings.globalTransformation);

  // throw exception if file can not be opened
  if( modFile.is_open() == false ) {

    throw std::runtime_error("Cannot open global transformation file.");

  }

  modFile >> modifications;
  modFile.close();

  MeshSequence sequence =
    MeshSequenceBuilder()
    .set_head_motion(headMotion)
    .set_registered_ema(registeredEma)
    .set_tongue_model(tongueModel)
    .set_global_transformation(modifications)
    .set_start_time(settings.startTime)
    .set_end_time(settings.endTime)
    .set_sampling_rate(settings.samplingRate)
    .set_scale_factor(settings.scaleFactor)
    .build();

  std::vector< std::pair<double, Mesh> > result =
    sequence.build();

  MeshSequenceOutput(result, settings.outputBase).output();

  return 0;

}
