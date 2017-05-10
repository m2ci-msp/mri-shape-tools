#ifndef __FACE_PAGE_EXPORTER_H__
#define __FACE_PAGE_EXPORTER_H__

#include <string>

#include <fstream>

#include <json/json.h>

#include "model/Model.h"
#include "XmlWriter.h"

class FacePageExporter{

public:

  /*--------------------------------------------------------------------------*/

  FacePageExporter(const Model& model) :
    model(model) {
  }

  /*--------------------------------------------------------------------------*/

  void write(const std::string& baseName) {

    this->baseName = baseName;

    write_mesh();
    write_tensor();
    write_mode_mean();
    write_descriptor();

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void write_mesh() {

    std::string fileName = this->baseName + ".xml";

    XmlWriter writer(this->model.data().get_shape_space_origin_mesh());

    writer.write_mesh_to(fileName);

  }

  /*--------------------------------------------------------------------------*/

  void write_descriptor() {

    std::string fileName = this->baseName + ".json";
    std::ofstream outFile;

    outFile.open(fileName);

    Json::Value descriptor(Json::objectValue);

    descriptor["totalNumberOfExpressions"] =
      this->model.data().get_original_phoneme_mode_dimension();
    descriptor["totalNumberOfIdentities"]  =
      this->model.data().get_original_speaker_mode_dimension();
    descriptor["truncatedNumberOfExpressions"] =
      this->model.data().get_phoneme_mode_dimension();
    descriptor["truncatedNumberOfIdentities"]  =
      this->model.data().get_speaker_mode_dimension();

    Json::StyledStreamWriter writer;
    writer.write(outFile, descriptor);

    outFile.close();

  }

  /*--------------------------------------------------------------------------*/

  void write_tensor() const {

    // build output file name
    std::string fileName = this->baseName + ".bin";

    const std::vector<double>& data =
      this->model.data().get_core_tensor().data().get_data();

    write_binary(data, fileName);

  } // end write_tensor

  /*--------------------------------------------------------------------------*/

  void write_mode_mean() const {

    // build output file name
    std::string fileName = this->baseName + "_modemean.bin";

    const arma::vec& speakerWeights =
      this->model.data().get_speaker_mean_weights();;

    const arma::vec& phonemeWeights =
      this->model.data().get_phoneme_mean_weights();;

    std::vector<double> data;

    for(const double& value: speakerWeights) {
      data.push_back(value);
    }
    for(const double& value: phonemeWeights) {
      data.push_back(value);
    }

    write_binary(data, fileName);

  }

  /*--------------------------------------------------------------------------*/

  void write_binary(
    const std::vector<double>& data,
    const std::string& fileName) const {

    std::ofstream output;

    output.open(fileName);


    for( const double& value: data) {

      union {
        float value;
        char bytes[sizeof(float)];
      } access;

      access.value = value;

      for( unsigned int i = 0; i < sizeof(float); ++i) {
        output.put(access.bytes[i]);
      } // end for i

    } // end for data

    output.close();


  }

  /*--------------------------------------------------------------------------*/

  Model model;
  std::string baseName;

};

#endif
