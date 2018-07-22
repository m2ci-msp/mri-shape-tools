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

      this->data.speakerWeights.push_back(speakerWeight);
      this->data.phonemeWeights.push_back(phonemeWeight);
      this->data.timeStamps.push_back(timeStamp);

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

  arma::vec build_vector(const Json::Value& value) const {

    arma::vec result = arma::zeros(value.size());

    for(unsigned int i = 0; i < value.size(); ++i ) {

      result(i) = value[i].asDouble();

    }

    return result;

  }

};
#endif
