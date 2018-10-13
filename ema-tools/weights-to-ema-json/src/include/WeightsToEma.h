#ifndef __WEIGHTS_TO_EMA_H__
#define __WEIGHTS_TO_EMA_H__

#include <string>
#include <fstream>

#include <json/json.h>
#include <armadillo>

#include "settings.h"

#include "model/ModelReader.h"
#include "model/Model.h"

#include "mesh/Mesh.h"

class WeightsToEma{

public:

  /*--------------------------------------------------------------------------*/

  WeightsToEma(const Settings& settings) : settings(settings) {

    init_ema_data();
    read_fit_data();
    read_model();

  }

  /*--------------------------------------------------------------------------*/

  void convert_and_save() {

    for(unsigned int i = 0; i < this->weightData.size(); ++i) {

      process_frame(i);

    }

    std::ofstream outFile(settings.output);

    Json::StyledStreamWriter writer;
    writer.write(outFile, this->emaData);

    outFile.close();

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void process_frame(const unsigned int& i) {

    Mesh mesh = reconstruct_current_mesh(i);

    add_time_stamp(this->weightData[i]["timeStamp"].asDouble());

    for(unsigned int i = 0; i < this->settings.sourceIds.size(); ++i) {

      // compute scaled coordinates
      const arma::vec point =
        this->settings.scale * mesh.get_vertices().at(this->settings.sourceIds.at(i));

      for(const double& value: point) {
        add_position_data(value, this->settings.channels.at(i));
      }

    }

    for(unsigned int i = 0; i < 3; ++i) {
      add_position_data(0, this->settings.reference);
    }

  }

  /*--------------------------------------------------------------------------*/

  Mesh reconstruct_current_mesh(const unsigned int& i) const {

    const Json::Value& speakerWeightsJson = this->weightData[i]["speakerWeights"];
    arma::vec speakerWeights = arma::zeros(speakerWeightsJson.size());

    for(unsigned int i = 0; i < speakerWeightsJson.size(); ++i) {
      speakerWeights(i) = speakerWeightsJson[i].asDouble();
    }

    const Json::Value& phonemeWeightsJson = this->weightData[i]["phonemeWeights"];
    arma::vec phonemeWeights = arma::zeros(phonemeWeightsJson.size());

    for(unsigned int i = 0; i < phonemeWeightsJson.size(); ++i) {
      phonemeWeights(i) = phonemeWeightsJson[i].asDouble();
    }

    return this->model.reconstruct_mesh().for_weights(speakerWeights, phonemeWeights);

  }

  /*--------------------------------------------------------------------------*/

  void read_fit_data() {

    // try to open file
    std::ifstream inFile(this->settings.input);

    // throw exception if file can not be opened
    if( inFile.is_open() == false ) {
      throw std::runtime_error("Cannot open weights file.");
    }

    // read frame data
    Json::Reader reader;
    reader.parse(inFile, this->weightData);


  }

  /*--------------------------------------------------------------------------*/

  void init_ema_data() {

    this->emaData["timeStamps"] = Json::Value(Json::arrayValue);
    this->emaData["channels"] = Json::Value(Json::objectValue);

    for(const std::string& channelName: settings.channels) {
      this->emaData["channels"][channelName] = Json::Value(Json::objectValue);
      this->emaData["channels"][channelName]["position"] = Json::Value(Json::arrayValue);
    }


  }

  /*--------------------------------------------------------------------------*/

  void read_model() {

    ModelReader modelReader(settings.model);
    this->model = modelReader.get_model();

  }

  /*--------------------------------------------------------------------------*/

  void add_position_data(const double& value, const std::string& channelName) {

    this->emaData["channels"][channelName]["position"].append(value);

  }

  /*--------------------------------------------------------------------------*/

  void add_time_stamp(const double& value) {

    this->emaData["timeStamps"].append(value);

  }

  /*--------------------------------------------------------------------------*/

  Json::Value weightData;
  Model model;

  Json::Value emaData;
  const Settings& settings;

  /*--------------------------------------------------------------------------*/

};

#endif
