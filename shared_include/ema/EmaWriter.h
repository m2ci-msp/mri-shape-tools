#ifndef __EMA_WRITER_H__
#define __EMA_WRITER_H__

#include <vector>
#include <string>
#include <map>
#include <fstream>

#include <armadillo>
#include <json/json.h>

#include "ema/EmaData.h"

class EmaWriter{

public:

  // constructor sets the reference to the main EmaData instance
  EmaWriter(const EmaData& emaData) : emaData(emaData) {
  
  }

  // build JSON object representing the current state of EmaData and write it to the given file
  void to(const std::string& fileName) const {
  
    std::ofstream outFile(fileName);
  
    Json::Value jsonEmaData(Json::objectValue);
  
    jsonEmaData["channels"] = build_coils(this->emaData.emaCoils);
    jsonEmaData["timestamps"] = build_time_stamps(this->emaData.emaInfoData.timeStamps);
    jsonEmaData["samplingFrequency"] = build_sampling_frequency(this->emaData.emaInfoData.samplingFrequency);
  
    outFile << jsonEmaData << std::endl;
  
    outFile.close();
  
  }

private:

  // builds JSON array containing the serialized positional data
  Json::Value build_positions(const EmaCoil& emaCoil ) const {
  
    Json::Value jsonPositions(Json::arrayValue);
  
    for(int i = 0; i < emaCoil.info().sample_amount(); ++i) {
  
      const arma::vec& position = emaCoil.access().position(i);
  
      jsonPositions.append(position[0]);
      jsonPositions.append(position[1]);
      jsonPositions.append(position[2]);
  
    }
  
    return jsonPositions;
  
  }

  // builds JSON array containing the serialized euler angle data
  Json::Value build_euler_angles(const EmaCoil& emaCoil ) const {
  
    Json::Value jsonEulerAngles(Json::arrayValue);
  
    for(int i = 0; i < emaCoil.info().sample_amount(); ++i) {
  
      const arma::vec& eulerAngle = emaCoil.access().euler_angle(i);
  
      jsonEulerAngles.append(eulerAngle[0]);
      jsonEulerAngles.append(eulerAngle[1]);
  
    }
  
    return jsonEulerAngles;
  
  }

  // builds JSON array containing the rms values
  Json::Value build_rms_values(const EmaCoil& emaCoil ) const {
  
    Json::Value jsonRmsValues(Json::arrayValue);
  
    for(int i = 0; i < emaCoil.info().sample_amount(); ++i) {
  
      const double rmsValue = emaCoil.access().rms_value(i);
  
      jsonRmsValues.append(rmsValue);
  
    }
  
    return jsonRmsValues;
  
  }

  // constructs JSON object representing a given coil
  Json::Value build_coil(const EmaCoil& emaCoil) const {
  
    Json::Value jsonCoil(Json::objectValue);
  
    jsonCoil["position"] = build_positions(emaCoil);
  
    if( emaCoil.info().euler_angles_present() ) {
  
      jsonCoil["eulerAngles"] = build_euler_angles(emaCoil);
  
    }
  
    if( emaCoil.info().rms_values_present() ) {
  
      jsonCoil["RMS"] = build_rms_values(emaCoil);
  
    }
  
    return jsonCoil;
  
  }

  // builds the JSON map containing the different coils
  Json::Value build_coils(const std::map<std::string, EmaCoil> emaCoils) const {
  
    Json::Value jsonCoils(Json::objectValue);
  
    for(const auto& coil: emaCoils) {
  
      jsonCoils[coil.first] = build_coil(coil.second);
  
    } 
  
    return jsonCoils;
  
  }

  // build the JSON array containing the time stamps
  Json::Value build_time_stamps(const std::vector<double> timeStamps) const {
  
    Json::Value jsonTimeStamps(Json::arrayValue);
  
    for(const double& timeStamp: timeStamps) {
  
      jsonTimeStamps.append(timeStamp);
  
    }
  
    return jsonTimeStamps;
  
  }

  // build JSON object representing the sampling frequency
  Json::Value build_sampling_frequency(const double& frequency) const {
  
    return Json::Value(frequency);
  
  }

  // reference to main EmaData instance
  const EmaData& emaData;

};
#endif
