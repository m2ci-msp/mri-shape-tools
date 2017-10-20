#ifndef __EMA_READER_H__
#define __EMA_READER_H__

#include <string>
#include <vector>
#include <map>
#include <istream>
#include <stdexcept>

#include <armadillo>
#include <json/json.h>

#include "ema/EmaData.h"

class EmaReader{

public:

  // constructor sets the reference to the main EmaData instance
  EmaReader(EmaData& emaData) : emaData(emaData) {
  
  }

  // opens a file, parses the JSON data, and builds the =EmaData= from this JSON object.
  void from(const std::string& fileName) {
  
    // try to open file
    std::ifstream inFile(fileName); 
  
    if( inFile.is_open() == false ) {
  
      throw std::runtime_error("Cannot open EMA file.");
  
    }
  
    // read json file
    Json::Value json;
    Json::Reader reader;
    reader.parse(inFile, json);
  
    inFile.close();
  
    // read necessary data
    this->emaData.emaCoils = read_coils( json["channels"] );
    this->emaData.timeStamps = read_time_stamps( json["timestamps"] );
    this->emaData.samplingFrequency = read_sampling_frequency( json["samplingFrequency"] );
    this->emaData.emaCoilIds = retrieve_coil_ids(this->emaData.emaCoils);
  
  }

private:

  // build emaCoils map from JSON object
  std::map<std::string, EmaCoil> read_coils(const Json::Value& jsonCoils) const {
  
    // get ids of coils from JSON object
    std::vector<std::string> emaCoilIds = jsonCoils.getMemberNames();
  
    std::map<std::string, EmaCoil> emaCoils;
  
    for(const std::string& id: emaCoilIds) {
  
      // construct individual coils
      emaCoils[id] = read_coil(jsonCoils[id]);
  
    }
  
    return emaCoils;
  
  }

  // construct single EmaCoil object from given JSON object
  EmaCoil read_coil(const Json::Value& jsonCoil) const {
  
    return EmaCoil( read_positions(jsonCoil["position"]) ); 
  
  }

  // read positional data from JSON object
  std::vector<arma::vec> read_positions(const Json::Value& jsonPositions) const {
  
    // perform integrity check
    if( jsonPositions.size() % 3 != 0 ) {
  
      throw std::runtime_error("Data size mismatch: Positional data samples array size is no mutliple of 3.");
  
    }
  
    // data is serialized -> divide by 3 to get sample amount
    int sampleAmount = jsonPositions.size() / 3;
  
    std::vector<arma::vec> positions;
  
    // iterate through data samples and reconstruct coordinates
    for(int i = 0; i < sampleAmount; ++i) {
  
      const double x = jsonPositions[3 * i + 0].asDouble();
      const double y = jsonPositions[3 * i + 1].asDouble();
      const double z = jsonPositions[3 * i + 2].asDouble();
  
      positions.push_back( {x, y, z} );
  
    }
  
    return positions;
  
  }

  // read time stamps from given JSON object
  std::vector<double> read_time_stamps(const Json::Value& jsonTimeStamps) {
  
    std::vector<double> timeStamps;
  
    for(Json::ArrayIndex i = 0; i < jsonTimeStamps.size(); ++i) {
  
      timeStamps.push_back(jsonTimeStamps[i].asDouble());
  
    }
  
    return timeStamps;
  
  }

  // read sampling frequency from JSON object
  double read_sampling_frequency(const Json::Value& jsonSamplingFrequency) const {
  
    return jsonSamplingFrequency.asDouble();
  
  }

  // gets coil IDs from build emaCoils map
  std::vector<std::string> retrieve_coil_ids( const std::map<std::string, EmaCoil>& emaCoils) const {
  
    std::vector<std::string> emaCoilIds;
  
    for(const auto& entry: emaCoils) {
  
      emaCoilIds.push_back(entry.first);
  
    }
  
    return emaCoilIds;
  
  }

  // reference to main EmaData instance
  EmaData& emaData;

};
#endif
