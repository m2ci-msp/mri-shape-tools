#ifndef __MODEL_DATA_H__
#define __MODEL_DATA_H__

#include <armadillo>

#include "mesh/Mesh.h"
#include "tensor/Tensor.h"

class ModelData{

public:

  /*--------------------------------------------------------------------------*/

  ModelData() {
  }

  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* member setters */
  /*--------------------------------------------------------------------------*/

  ModelData& set_core_tensor(const Tensor& coreTensor) {

    this->coreTensor = coreTensor;

    return *this;
  }

  /*--------------------------------------------------------------------------*/

  ModelData& set_shape_space_origin(const arma::vec& origin) {

    this->shapeSpaceOrigin = origin;

    return *this;
  }

  /*--------------------------------------------------------------------------*/

  ModelData& set_shape_space_origin_mesh(const Mesh& mesh) {

    this->shapeSpaceOriginMesh = mesh;

    return *this;
  }

  /*--------------------------------------------------------------------------*/


  ModelData& set_speaker_mean_weights(const arma::vec& meanWeights) {

    this->speakerMeanWeights = meanWeights;

    return *this;
  }

  /*--------------------------------------------------------------------------*/

  ModelData& set_phoneme_mean_weights(
    const arma::vec& meanWeights) {

    this->phonemeMeanWeights = meanWeights;

    return *this;
  }

  /*--------------------------------------------------------------------------*/

  ModelData& set_original_speaker_mode_dimension(
    const int& speakerDimension
    ) {

    this->originalSpeakerModeDimension = speakerDimension;

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  ModelData& set_original_phoneme_mode_dimension(
    const int& phonemeDimension
    ) {

    this->originalPhonemeModeDimension = phonemeDimension;

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  ModelData& set_sample_matrices(
    const arma::mat& speakerMatrix, const arma::mat& phonemeMatrix) {

    this->speakerSamplingMatrix = speakerMatrix;
    this->phonemeSamplingMatrix = phonemeMatrix;

    this->isSamplingPossible = true;

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  ModelData& set_original_weights(
    const arma::mat& originalSpeakerWeights,
    const arma::mat& originalPhonemeWeights
    ) {

    this->originalSpeakerWeights = originalSpeakerWeights;
    this->originalPhonemeWeights = originalPhonemeWeights;

    this->areOriginalWeightsAvailable = true;

    return *this;

  }


  /*--------------------------------------------------------------------------*/
  /* member getters */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/

  const Tensor& get_core_tensor() const {
    return this->coreTensor;
  }

  /*--------------------------------------------------------------------------*/

  const arma::vec& get_shape_space_origin() const {
    return this->shapeSpaceOrigin;
  }

  /*--------------------------------------------------------------------------*/

  const Mesh& get_shape_space_origin_mesh() const {
    return this->shapeSpaceOriginMesh;
  }

  /*--------------------------------------------------------------------------*/

  const arma::vec& get_speaker_mean_weights() const {
    return this->speakerMeanWeights;
  }

  /*--------------------------------------------------------------------------*/

  const arma::vec& get_phoneme_mean_weights() const {
    return this->phonemeMeanWeights;
  }

  /*--------------------------------------------------------------------------*/

  int get_speaker_mode_dimension() const {
    return this->coreTensor.data().get_mode_one_dimension();
  }

  /*--------------------------------------------------------------------------*/

  int get_phoneme_mode_dimension() const {
    return this->coreTensor.data().get_mode_two_dimension();
  }

  /*--------------------------------------------------------------------------*/

  int get_vertex_mode_dimension() const {
    return this->coreTensor.data().get_mode_three_dimension();
  }

  /*--------------------------------------------------------------------------*/

  int get_original_speaker_mode_dimension() const {
    return this->originalSpeakerModeDimension;
  }

  /*--------------------------------------------------------------------------*/

  int get_original_phoneme_mode_dimension() const {
    return this->originalPhonemeModeDimension;
  }

  /*--------------------------------------------------------------------------*/

  const arma::mat& get_speaker_sampling_matrix() const {
    return this->speakerSamplingMatrix;
  }

  /*--------------------------------------------------------------------------*/

  const arma::mat& get_original_speaker_weights() const {
    return this->originalSpeakerWeights;
  }

  /*--------------------------------------------------------------------------*/

  const arma::mat& get_original_phoneme_weights() const {
    return this->originalPhonemeWeights;
  }

  /*--------------------------------------------------------------------------*/

  const arma::mat& get_phoneme_sampling_matrix() const {
    return this->phonemeSamplingMatrix;
  }

  /*--------------------------------------------------------------------------*/

  bool is_sampling_possible() const {
    return this->isSamplingPossible;
  }

  /*--------------------------------------------------------------------------*/

  bool are_original_weights_available() const {
    return this->areOriginalWeightsAvailable;
  }

  /*--------------------------------------------------------------------------*/

  void mark_as_truncated() {
    this->isTruncated = true;
  }

  /*--------------------------------------------------------------------------*/

  bool is_truncated() const {
    return this->isTruncated;
  }

  /*--------------------------------------------------------------------------*/



private:

  /*--------------------------------------------------------------------------*/

  Tensor coreTensor;

  arma::vec shapeSpaceOrigin;
  arma::vec speakerMeanWeights;
  arma::vec phonemeMeanWeights;

  Mesh shapeSpaceOriginMesh;

  // original sizes of the modes before truncation was applied
  int originalSpeakerModeDimension;
  int originalPhonemeModeDimension;

  /*--------------------------------------------------------------------------*/

  // optional members, only present if model was built

  // sampling matrices
  arma::mat speakerSamplingMatrix;
  arma::mat phonemeSamplingMatrix;

  // original speaker mode weights as row vectors
  arma::mat originalSpeakerWeights;

  // original phoneme mode weights as row vectors
  arma::mat originalPhonemeWeights;

  bool isSamplingPossible = false;
  bool areOriginalWeightsAvailable = false;

  bool isTruncated  = false;

  /*--------------------------------------------------------------------------*/

};

#endif
