#ifndef __EMA_DATA_H__
#define __EMA_DATA_H__

#include <string>
#include <fstream>
#include <stdexcept>

#include <json/json.h>

#include "mesh/Mesh.h"

class EmaData{

public:

  /*--------------------------------------------------------------------------*/

  EmaData(const Settings& settings) : settings(settings) {

    // parse json file
    std::ifstream inFile(this->settings.inputFileName);

    // throw exception if file can not be opened
    if( inFile.is_open() == false ) {
      throw std::runtime_error("Cannot open json file.");
    }

    Json::Reader reader;
    reader.parse(inFile, this->json);

    inFile.close();

    setup_channel_data();

    setup_reference_data();

    if( this->settings.enforceMidsagittal == true ) {
      enforce_midsagittal();
    }

    this->shift = arma::zeros(3);

  }

  /*--------------------------------------------------------------------------*/

  void set_shift(const arma::vec& shift) {

    this->shift = shift;

  }

  /*--------------------------------------------------------------------------*/

  void set_scale_factor(const double& scaleFactor) {

    this->scaleFactor = scaleFactor;

  }

  /*--------------------------------------------------------------------------*/

  Mesh get_mesh_at(const int& timeIndex) {


    // compute index for accessing the positional data:
    // timeIndex * 3 ( x, y, and z coordinates )
    Json::ArrayIndex index = timeIndex * 3;

    // get position of reference coil
    const arma::vec reference ( {
        this->referenceData[index + 0].asDouble(),
          this->referenceData[index + 1].asDouble(),
          this->referenceData[index + 2].asDouble()
          } );

    // get positions of wanted channels at wanted time step
    std::vector<arma::vec> vertices;

    for( const Json::Value& positions: this->channelData ) {

      arma::vec point( {
            positions[index + 0].asDouble(),
            positions[index + 1].asDouble(),
            positions[index + 2].asDouble()
            });

      // map point to reference
      point -= reference;

      point *= this->scaleFactor;

      point -= shift;

      vertices.push_back(point);

    } // end for

    Mesh result;
    result.set_vertices(vertices);

    return result;

  } // end get_mesh_at

  /*--------------------------------------------------------------------------*/

  double get_time_at(const int& timeIndex) {

    return this->timeStamps[timeIndex].asDouble();

  }

  /*--------------------------------------------------------------------------*/

  int get_size() const {
    return this->size;
  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void setup_channel_data() {

    // extract positional data for wanted channels
    for(const std::string& name: this->settings.channels) {
      this->channelData.push_back(extract_channel_data(name, this->json));
    }

    // compute size
    this->size = this->channelData.at(0).size() / 3;

    // save time stamps
    this->timeStamps = json["timestamps"];

  }

  /*--------------------------------------------------------------------------*/

  void setup_reference_data() {

    // generate positional data of reference coil
    if(this->settings.referencePresent == true ) {

      // use provided data if present
      this->referenceData = extract_channel_data(this->settings.reference, this->json);

    } else {

      // otherwise, generate reference data with (0, 0, 0) positions
      this->referenceData = Json::arrayValue;

      for(int i = 0; i < 3 * this->size; ++i) {
        this->referenceData.append(0);
      }

    } // end else

  }

  /*--------------------------------------------------------------------------*/

  void enforce_midsagittal() {

    for( Json::Value& positions: this->channelData ) {

      for( int i = 0; i < this->size * 3; ++i) {

        if( i % 3 == 0 ) { positions[i] = 0; }

      } // end for i

    } // end for positions

  }

  /*--------------------------------------------------------------------------*/

  Json::Value extract_channel_data(
    const std::string& channelName,
    const Json::Value& json
    ) const {

    // try to get channels
    const Json::Value channels = get_data("channels", json);

    // try to get channel
    const Json::Value channel = get_data(channelName, channels);

    // try to get positional data
    const Json::Value positions = get_data("position", channel);

    return positions;

  }

  /*--------------------------------------------------------------------------*/

  Json::Value get_data( const std::string& id, const Json::Value& value) const {

    const Json::Value result = value[id];

    if(result.isNull()) {
      throw std::runtime_error("Can not access " + id + " data in json file.");
    }

    return result;

  }

  /*--------------------------------------------------------------------------*/

  Json::Value json;

  const Settings& settings;

  std::vector<Json::Value> channelData;

  Json::Value referenceData;
  Json::Value timeStamps;

  int size = 0;

  double scaleFactor = 1.;

  arma::vec shift;

  /*--------------------------------------------------------------------------*/

};

#endif
