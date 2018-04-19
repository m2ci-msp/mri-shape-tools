#include <fstream>
#include <json/json.h>

#include "tensor/Tensor.h"
#include "tensor/TensorBuilder.h"

#include "training/SampleFileReader.h"
#include "training/TrainingDataBuilder.h"

#include "Compactness.h"
#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  SampleDataBase database = SampleFileReader::read_from(settings.input);

  TrainingDataBuilder trainingBuilder(database);

  TrainingData data = trainingBuilder.build();
  Tensor tensor = TensorBuilder::build_tensor(data);

  Compactness compactness(tensor);

  std::vector<double> speakerCompactness = compactness.for_speaker_mode();
  std::vector<double> phonemeCompactness = compactness.for_phoneme_mode();

  Json::Value speakerJson(Json::arrayValue);

  for(unsigned int i = 0; i < speakerCompactness.size(); ++i) {

    Json::Value entry(Json::objectValue);

    entry["components"] = i + 1;
    entry["variability"] = speakerCompactness.at(i);

    speakerJson.append(entry);

  }

  std::ofstream outFile(settings.output + "_speaker.json");

  outFile << speakerJson << std::endl;
  outFile.close();

  Json::Value phonemeJson(Json::arrayValue);

  for(unsigned int i = 0; i < phonemeCompactness.size(); ++i) {

    Json::Value entry(Json::objectValue);

    entry["components"] = i + 1;
    entry["variability"] = phonemeCompactness.at(i);

    phonemeJson.append(entry);

  }


  outFile = std::ofstream(settings.output + "_phoneme.json");

  outFile << phonemeJson << std::endl;
  outFile.close();

  return 0;

}
