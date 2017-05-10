#ifndef __MODEL_BUILDER_H__
#define __MODEL_BUILDER_H__

#include <stdexcept>

#include <armadillo>

#include "model/ModelData.h"
#include "model/Model.h"

#include "tensor/Tensor.h"
#include "tensor/TensorAnalysis.h"

#include "utility/Serializer.h"

#include "mesh/Mesh.h"

class ModelBuilder{

public:

  /*--------------------------------------------------------------------------*/

  ModelBuilder() {

    this->tensorSet = false;
    this->originSet = false;
    this->facesSet = false;

  }

  /*--------------------------------------------------------------------------*/

  void set_tensor(const Tensor& tensor) {

    this->tensor = tensor;

    // save original mode dimensions
    this->originalSpeakerModeDimension =
      this->tensor.data().get_mode_one_dimension();

    this->originalPhonemeModeDimension =
      this->tensor.data().get_mode_two_dimension();

    // set truncated mode dimensions to original ones
    this->truncatedSpeakerModeDimension =
      this->tensor.data().get_mode_one_dimension();

    this->truncatedPhonemeModeDimension =
      this->tensor.data().get_mode_two_dimension();

    this->tensorSet = true;

  }

  /*--------------------------------------------------------------------------*/

  void set_origin(const arma::vec& origin) {

    this->origin = origin;
    this->originSet = true;

  }

  /*--------------------------------------------------------------------------*/

  void set_faces(
    const std::vector< std::vector<unsigned int> > faces) {

    this->faces = faces;
    this->facesSet = true;

  }

  /*--------------------------------------------------------------------------*/

  void set_truncated_speaker_mode_dimension(const int& truncatedDimension) {

    this->truncatedSpeakerModeDimension = truncatedDimension;

  }

  /*--------------------------------------------------------------------------*/

  void set_truncated_phoneme_mode_dimension(const int& truncatedDimension) {

    this->truncatedPhonemeModeDimension = truncatedDimension;

  }

  /*--------------------------------------------------------------------------*/


  Model build() {

    // verify that necessary data is present
    verify();

    construct_origin_mesh();

    TensorAnalysis analysis(this->tensor);

    analysis.set_truncated_mode_one_dimension(
      this->truncatedSpeakerModeDimension
      );

    analysis.set_truncated_mode_two_dimension(
      this->truncatedPhonemeModeDimension
      );

    Tensor coreTensor = analysis.get_core_tensor();
    arma::rowvec speakerMeanWeights = analysis.get_mode_one_mean();
    arma::rowvec phonemeMeanWeights = analysis.get_mode_two_mean();

    const arma::mat originalSpeakerWeights = analysis.get_mode_one_U();
    const arma::mat originalPhonemeWeights = analysis.get_mode_two_U();

    // matrices that can be used to generate a sample from the
    // multivariate Gaussian distributions of the two modes

    const arma::mat speakerSamplingMatrix =
      estimate_sampling_matrix(
        originalSpeakerWeights,
        speakerMeanWeights
        );

    const arma::mat phonemeSamplingMatrix =
      estimate_sampling_matrix(
        originalPhonemeWeights,
        phonemeMeanWeights
        );

    ModelData modelData;
    modelData.set_core_tensor(coreTensor)                     \
      .set_shape_space_origin(this->origin)                   \
      .set_shape_space_origin_mesh(this->originMesh)          \
      .set_speaker_mean_weights(speakerMeanWeights.t())       \
      .set_phoneme_mean_weights(phonemeMeanWeights.t())       \
      .set_original_speaker_mode_dimension(this->originalSpeakerModeDimension) \
      .set_original_phoneme_mode_dimension(this->originalPhonemeModeDimension) \
      .set_sample_matrices(speakerSamplingMatrix, phonemeSamplingMatrix) \
      .set_original_weights(originalSpeakerWeights, originalPhonemeWeights);

    return Model(modelData);

  }

  /*--------------------------------------------------------------------------*/

  void construct_origin_mesh() {

    this->originMesh.set_vertices(Serializer::unserialize(this->origin));
    this->originMesh.set_faces(this->faces);

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  arma::mat estimate_sampling_matrix(
    arma::mat samples,
    const arma::rowvec& mean
    ) const {

    // center samples
    samples.each_row() -= mean;

    // compute covariance matrix
    arma::mat X = arma::cov(samples, samples, 1);

    // perform eigenvalue decomposition
    arma::mat eigvec;
    arma::vec eigval;

    arma::eig_sym(eigval, eigvec, X);

    // take square root of eigenvalues
    // make sure that we do not have negative eigenvalues that might
    // occur due to numerical errors
    for(double& entry: eigval) {
      entry = (entry < 0)? 0 : sqrt(entry);
    }

    // construct matrix suitable for transforming a unit random vector
    // to the estimated normal distribution
    // cf. e.g. https://en.wikipedia.org/wiki/Multivariate_normal_distribution#Drawing_values_from_the_distribution
    return eigvec * arma::diagmat(eigval);

  }
  /*--------------------------------------------------------------------------*/

  void verify() {

    if( !(this->tensorSet && this->facesSet && this->originSet) ) {

      throw std::logic_error(
        "Can not build model. Necessary data is not present.");

    } // end if

  }

  /*--------------------------------------------------------------------------*/

  Mesh originMesh;

  Tensor tensor;

  arma::vec origin;
  std::vector< std::vector<unsigned int> > faces;

  /*--------------------------------------------------------------------------*/

  bool tensorSet;
  bool originSet;
  bool facesSet;

  /*--------------------------------------------------------------------------*/

  int originalSpeakerModeDimension;
  int originalPhonemeModeDimension;

  int truncatedSpeakerModeDimension;
  int truncatedPhonemeModeDimension;

  /*--------------------------------------------------------------------------*/

};

#endif
