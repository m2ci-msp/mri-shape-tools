#ifndef __MODEL_READER_H__
#define __MODEL_READER_H__

#include <vector>

#include <yaml-cpp/yaml.h>

#include "tensor/Tensor.h"
#include "tensor/TensorData.h"

#include "model/Model.h"
#include "model/ModelData.h"

#include "utility/Serializer.h"
#include "utility/BinaryConverter.h"

class ModelReader{

public:

  /*--------------------------------------------------------------------------*/

  ModelReader(const std::string& fileName) {

    this->modelFile = YAML::LoadFile(fileName);

    read_dimensions();
    read_core_tensor();
    read_mean_weights();
    read_shape_space_information();

  }


  /*--------------------------------------------------------------------------*/

  Model get_model() {

    const Tensor coreTensor = build_core_tensor();
    const Mesh originShape = build_origin_shape_mesh();

    ModelData modelData;

    modelData.set_core_tensor(coreTensor) \
      .set_speaker_mean_weights(this->speakerMeanWeights) \
      .set_phoneme_mean_weights(this->phonemeMeanWeights) \
      .set_shape_space_origin(this->origin) \
      .set_shape_space_origin_mesh(originShape) \
      .set_original_speaker_mode_dimension(this->dimensionOriginalSpeakerMode) \
      .set_original_phoneme_mode_dimension(this->dimensionOriginalPhonemeMode);

    return Model(modelData);

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void read_dimensions() {

    const YAML::Node& modeDims = this->modelFile["Dimensions"];

    this->dimensionOriginalSpeakerMode =
      modeDims["OriginalSpeakerMode"].as<int>();
    this->dimensionOriginalPhonemeMode =
      modeDims["OriginalPhonemeMode"].as<int>();

    this->dimensionSpeakerMode = modeDims["TruncatedSpeakerMode"].as<int>();
    this->dimensionPhonemeMode = modeDims["TruncatedPhonemeMode"].as<int>();

    this->dimensionVertexMode = modeDims["VertexMode"].as<int>();

    return;

  }

  /*--------------------------------------------------------------------------*/

  void read_core_tensor() {

    if( this->modelFile["CoreTensor"].IsScalar() ) {

      const YAML::Binary& binaryNode = this->modelFile["CoreTensor"].as<YAML::Binary>();

      this->coreTensorData =
        BinaryConverter::convert_to_double(
          binaryNode.data(), binaryNode.size());

    }
    else if( this->modelFile["CoreTensor"].IsSequence() ) {

      this->coreTensorData = this->modelFile["CoreTensor"].as< std::vector<double> >();

    }
    else {

      throw std::runtime_error("Can not read core tensor data.");

    }

    return;

  }

  /*--------------------------------------------------------------------------*/

  void read_mean_weights() {

    std::vector<double> speakerWeights;
    std::vector<double> phonemeWeights;

    const YAML::Node& modeMeanWeights =
      this->modelFile["MeanWeights"];

    for( const YAML::Node& value: modeMeanWeights["SpeakerMode"]){
      speakerWeights.push_back(value.as<double>());
    }

    for( const YAML::Node& value: modeMeanWeights["PhonemeMode"]){
      phonemeWeights.push_back(value.as<double>());
    }

    this->speakerMeanWeights = arma::vec(speakerWeights);
    this->phonemeMeanWeights = arma::vec(phonemeWeights);

    return;

  } // end read_mean_weights

  /*--------------------------------------------------------------------------*/

  void read_shape_space_information() {

    const YAML::Node& shapeSpace = this->modelFile["ShapeSpace"];

    std::vector<double> originData;

    for(const YAML::Node& value: shapeSpace["Origin"]) {
      originData.push_back(value.as<double>());
    }

    this->origin = arma::vec(originData);


    // read face information
    for( const YAML::Node& faceData: shapeSpace["Faces"] ) {

      std::vector< unsigned int > face;

      for( const YAML::Node& index: faceData) {
        face.push_back(index.as<unsigned int>());
      } // end for index

      this->faces.push_back(face);

    } // end for faceData


  }

  /*--------------------------------------------------------------------------*/

  Mesh build_origin_shape_mesh() const {

    Mesh mesh;
    mesh.set_vertices(Serializer::unserialize(this->origin));
    mesh.set_faces(this->faces);

    return mesh;

  }

  /*--------------------------------------------------------------------------*/

  Tensor build_core_tensor() const {

    TensorData tensorData;

    tensorData.set_data(this->coreTensorData)       \
      .set_mode_dimensions(
        this->dimensionSpeakerMode,
        this->dimensionPhonemeMode,
        this->dimensionVertexMode
        );

    return Tensor(tensorData);

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  // dimensions
  int dimensionOriginalSpeakerMode;
  int dimensionOriginalPhonemeMode;
  int dimensionSpeakerMode;
  int dimensionPhonemeMode;
  int dimensionVertexMode;

  // core tensor
  std::vector<double> coreTensorData;

  // mean weights
  arma::vec speakerMeanWeights;
  arma::vec phonemeMeanWeights;

  // shape space information
  arma::vec origin;
  std::vector< std::vector<unsigned int> > faces;

  /*--------------------------------------------------------------------------*/

  YAML::Node modelFile;

  /*--------------------------------------------------------------------------*/

};
#endif
