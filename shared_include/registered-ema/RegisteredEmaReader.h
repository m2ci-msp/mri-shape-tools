#ifndef __REGISTERED_EMA_READER_H__
#define __REGISTERED_EMA_READER_H__

#include <istream>
#include <stdexcept>

#include <armadillo>
#include <json/json.h>

#include "RegisteredEmaData.h"

class RegisteredEmaReader{

private:

  RegisteredEmaData& data;

public:

  RegisteredEmaReader(RegisteredEmaData& data) :

    data(data) {

  }

  void from(const std::string& fileName) {

    // try to open file
    std::ifstream inFile(fileName);

    if(inFile.is_open() == false) {

      throw std::runtime_error("Cannot open registered EMA file.");

    }

    // read json file
    Json::Value json;

    inFile >> json;

    inFile.close();

    // erase data
    this->data.speakerWeights.clear();
    this->data.phonemeWeights.clear();
    this->data.timeStamps.clear();
    this->data.boundarySize = 0;

    process_frame_list(json);

  }

private:

  void process_frame_list(const Json::Value& frames) {

    for(const Json::Value& frame: frames) {

      const arma::vec speakerWeight = get_speaker_weight(frame);
      const arma::vec phonemeWeight = get_phoneme_weight(frame);

      const double timeStamp = get_time_stamp(frame);

      std::vector<arma::vec> targetPoints = get_target_points(frame);

      this->data.speakerWeights.push_back(speakerWeight);
      this->data.phonemeWeights.push_back(phonemeWeight);
      this->data.timeStamps.push_back(timeStamp);
      this->data.targetPoints.push_back(targetPoints);

    }

  }

  arma::vec get_speaker_weight(const Json::Value& frame) const {

    return build_vector(frame["speakerWeights"]);

  }

  arma::vec get_phoneme_weight(const Json::Value& frame) const {

    return build_vector(frame["phonemeWeights"]);

  }

  double get_time_stamp(const Json::Value& frame) const {

    return frame["timeStamp"].asDouble();

  }

  std::vector<arma::vec> get_target_points(const Json::Value& frame) const {

    std::vector<arma::vec> targetPoints;

    for(const Json::Value& jsonPoint: frame["target"] ) {

      const double x = jsonPoint["x"].asDouble();
      const double y = jsonPoint["y"].asDouble();
      const double z = jsonPoint["z"].asDouble();

      targetPoints.push_back({x, y, z});

    }

    return targetPoints;

  }

  arma::vec build_vector(const Json::Value& value) const {

    arma::vec result = arma::zeros(value.size());

    for(unsigned int i = 0; i < value.size(); ++i ) {

      result(i) = value[i].asDouble();

    }

    return result;

  }

};
#endif
