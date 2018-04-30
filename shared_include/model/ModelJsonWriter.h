#ifndef __MODEL_JSON_WRITER_H__
#define __MODEL_JSON_WRITER_H__

#include <string>

#include <armadillo>
#include <json/json.h>

#include "model/Model.h"

class ModelJsonWriter{

public:

  /*--------------------------------------------------------------------------*/

  ModelJsonWriter(const Model& model) : model(model) {

    output_dimensions();
    output_core_tensor();
    output_mean_weights();
    output_shape_space_information();

  }

  /*--------------------------------------------------------------------------*/

  void write(const std::string& fileName) {

    std::ofstream outFile(fileName);

    outFile << this->root << std::endl;

    outFile.close();

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void output_dimensions() {

    const int dimensionPhonemeMode =
      this->model.data().get_phoneme_mode_dimension();
    const int dimensionSpeakerMode =
      this->model.data().get_speaker_mode_dimension();
    const int dimensionVertexMode =
      this->model.data().get_vertex_mode_dimension();

    const int dimensionOriginalSpeakerMode =
      this->model.data().get_original_speaker_mode_dimension();

    const int dimensionOriginalPhonemeMode =
      this->model.data().get_original_phoneme_mode_dimension();

    Json::Value dimensions;

    dimensions["OriginalSpeakerMode"] = dimensionOriginalSpeakerMode;
    dimensions["OriginalPhonemeMode"] = dimensionOriginalPhonemeMode;
    dimensions["TruncatedSpeakerMode"] = dimensionSpeakerMode;
    dimensions["TruncatedPhonemeMode"] = dimensionPhonemeMode;
    dimensions["VertexMode"] = dimensionVertexMode;

    this->root["Dimensions"] = dimensions;

  } // end output_dimensions

  /*--------------------------------------------------------------------------*/

  void output_core_tensor() {

    const std::vector<double>& coreTensor =
      this->model.data().get_core_tensor().data().get_data();

    Json::Value tensor(Json::arrayValue);

    for(const double& value: coreTensor) {

      tensor.append(value);

    }

    this->root["CoreTensor"] = tensor;

  } // end output_core_tensor

  /*--------------------------------------------------------------------------*/

  void output_mean_weights() {

    const arma::vec& phonemeMean =
      this->model.data().get_phoneme_mean_weights();
    const arma::vec& speakerMean =
      this->model.data().get_speaker_mean_weights();

    Json::Value meanWeights;

    meanWeights["SpeakerMode"] = Json::arrayValue;

    for(const double& value: speakerMean) {

      meanWeights["SpeakerMode"].append(value);

    }

    meanWeights["PhonemeMode"] = Json::arrayValue;

    for(const double& value: phonemeMean) {

      meanWeights["PhonemeMode"].append(value);

    }

    this->root["MeanWeights"] = meanWeights;

  } // end output_mean_weights

  /*--------------------------------------------------------------------------*/

  void output_shape_space_information() {

    const arma::vec& origin =
      this->model.data().get_shape_space_origin();
    const std::vector< std::vector< unsigned int > >& faces =
      this->model.data().get_shape_space_origin_mesh().get_faces();

    Json::Value shapeSpace;

    shapeSpace["Origin"] = Json::arrayValue;

    for(const double& value: origin) {

      shapeSpace["Origin"].append(value);

    }

    shapeSpace["Faces"] = Json::arrayValue;

    for(const auto& face: faces) {

      Json::Value jsonFace(Json::arrayValue);

      for( const unsigned index: face) {

        jsonFace.append(index);

      } // end for face

      shapeSpace["Faces"].append(jsonFace);

    } // end for faces

    this->root["ShapeSpace"] = shapeSpace;


  } // end output_shape_space_information

  /*--------------------------------------------------------------------------*/

  const Model& model;
  Json::Value root;

  /*--------------------------------------------------------------------------*/

};

#endif
