#include <yaml-cpp/yaml.h>

#include "mesh/MeshIO.h"

#include "tensor/TensorBuilder.h"
#include "model/ModelBuilder.h"
#include "training/SampleFileReader.h"
#include "training/TrainingDataBuilder.h"
#include "SpeakerSpecificityMeasurer.h"
#include "PhonemeSpecificityMeasurer.h"
#include "ErrorDatabaseWriter.h"

#include "SubsetCollection.h"

#include "settings.h"

#include "Specificity.h"


int main(int argc, char* argv[]){

  Settings settings(argc, argv);

  Specificity specificity(settings);

  specificity.measure();

  return 0;

}
