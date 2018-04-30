#include "mesh/MeshIO.h"

#include "tensor/Tensor.h"
#include "tensor/TensorBuilder.h"

#include "training/SampleFileReader.h"
#include "training/TrainingDataBuilder.h"

#include "model/Model.h"
#include "model/ModelBuilder.h"
#include "model/ModelWriter.h"

#include "model/ModelJsonWriter.h"
#include "settings.h"

void write_phoneme_weights(Model& model, const SampleDataBase& database, const std::string& fileName ) {

  YAML::Emitter emitter;

  emitter << YAML::BeginMap;

  const arma::mat originalWeights = model.data().get_original_phoneme_weights();

  unsigned int i = 0;

  for(const std::string& phonemeId: database.get_phoneme_ids()) {

    const arma::vec phonemeWeights = originalWeights.row(i).t();

    emitter << YAML::Key << phonemeId
            << YAML::Value << YAML::Flow << YAML::BeginSeq;

    for(const double& value: phonemeWeights) {
      emitter << value;
    }

    emitter << YAML::EndSeq;

    // increment index for weight access
    ++i;
  }

  emitter << YAML::EndMap;

  std::ofstream outFile(fileName);
  outFile << emitter.c_str();
  outFile.close();

}

int main(int argc, char* argv[]){

  Settings settings(argc, argv);

  SampleDataBase database = SampleFileReader::read_from(settings.samples);
  TrainingDataBuilder trainingBuilder(database);

  TrainingData data = trainingBuilder.build();

  auto tensor = TensorBuilder::build_tensor(data);

  ModelBuilder builder;

  builder.set_faces(database.get_faces());
  builder.set_tensor(tensor);
  builder.set_origin(data.mean);

  if( settings.truncateSpeaker ) {

    builder.set_truncated_speaker_mode_dimension(settings.truncatedSpeakerDimension);

  }

  if( settings.truncatePhoneme ) {

    builder.set_truncated_phoneme_mode_dimension(settings.truncatedPhonemeDimension);

  }

  Model model = builder.build();

  ModelWriter writer(model);
  writer.write(settings.output);

  if( settings.outputMeanMesh ) {

    MeshIO::write(model.data().get_shape_space_origin_mesh(), settings.outputMeanMeshFile);

  }

  if( settings.outputPhonemeWeights ) {

    write_phoneme_weights(model, database, settings.outputPhonemeWeightsFile);

  }

  if( settings.outputJson ) {

    ModelJsonWriter(model).write(settings.jsonOutput);

  }

  return 0;

}
