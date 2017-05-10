#ifndef __MODEL_SPACE_H__
#define __MODEL_SPACE_H__

#include <vector>

#include <armadillo>

#include "model/ModelData.h"

class ModelSpace{

public:

  /*--------------------------------------------------------------------------*/

  ModelSpace(const ModelData& modelData) : modelData(modelData) {

    update();

  }

  /*--------------------------------------------------------------------------*/

  void update() {
    generate_model_speaker();
    generate_model_phoneme();
  }

  /*--------------------------------------------------------------------------*/

  // use speaker weights to remove this degree of freedom
  // basically, this is the mode one multiplication of the core tensor with
  // a vector x:
  //
  // r = x^T S V^T
  //
  // where
  // - S: singular values of the mode
  // - V: right singular vectors of mode
  arma::mat speaker(const arma::vec& speakerWeights) const {

    // get dimensions
    const int& dimensionVertexMode =
      this->modelData.get_vertex_mode_dimension();

    const int& dimensionPhonemeMode =
      this->modelData.get_phoneme_mode_dimension();

    arma::mat speaker(
      dimensionVertexMode,
      dimensionPhonemeMode, arma::fill::zeros);

    for(unsigned int i = 0; i < modelSpeaker.size(); ++i) {
      speaker += speakerWeights(i) * modelSpeaker.at(i);
    }

    return speaker;

  }

  /*--------------------------------------------------------------------------*/

  // use phoneme weights to remove this degree of freedom
  // basically, this is the mode two multiplication with a vector
  arma::mat phoneme(const arma::vec& phonemeWeights) const {

    // get dimensions
    const int& dimensionVertexMode =
      this->modelData.get_vertex_mode_dimension();

    const int& dimensionSpeakerMode =
      this->modelData.get_speaker_mode_dimension();

    arma::mat phoneme(
      dimensionVertexMode,
      dimensionSpeakerMode, arma::fill::zeros);

    // generate space
    for(unsigned int i = 0; i < modelPhoneme.size(); ++i) {
      phoneme += phonemeWeights(i) * this->modelPhoneme.at(i);
    }

    return phoneme;

  }

  /*--------------------------------------------------------------------------*/

  void set_model_speaker(const std::vector<arma::mat>& modelSpeaker) {
    this->modelSpeaker = modelSpeaker;
  }

  /*--------------------------------------------------------------------------*/

  void set_model_phoneme(const std::vector<arma::mat>& modelPhoneme) {
    this->modelPhoneme = modelPhoneme;
  }

  /*--------------------------------------------------------------------------*/

  const std::vector<arma::mat>& get_model_speaker() const {
    return this->modelSpeaker;
  }

  /*--------------------------------------------------------------------------*/

  const std::vector<arma::mat>& get_model_phoneme() const {
    return this->modelPhoneme;
  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void generate_model_speaker() {

    this->modelSpeaker =
      this->modelData.get_core_tensor().modes().\
      get_mode_three_matrices_along_mode_one();

  }

  /*--------------------------------------------------------------------------*/

  void generate_model_phoneme() {

    this->modelPhoneme =
      this->modelData.get_core_tensor().modes().\
      get_mode_three_matrices_along_mode_two();

  }

  /*--------------------------------------------------------------------------*/

  const ModelData& modelData;

  // vector of vertexModeDimension x phonemeModeDimension matrices
  std::vector<arma::mat> modelSpeaker;

  // vector of vertexModeDimension x speakerModeDimension matrices
  std::vector<arma::mat> modelPhoneme;

  /*--------------------------------------------------------------------------*/

};
#endif
