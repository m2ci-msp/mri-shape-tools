#ifndef __MODEL_SAMPLER_H__
#define __MODEL_SAMPLER_H__

#include <chrono>
#include <random>
#include <stdexcept>

#include "model/ModelData.h"
#include "model/ModelMeshReconstructor.h"

class ModelSampler{

public:

  /*-----------------------------------------------------------------------*/

  ModelSampler(const ModelData& modelData, const ModelMeshReconstructor& meshReconstructor) :
    modelData(modelData), reconstructMesh(meshReconstructor) {

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    this->generator = std::default_random_engine(seed);

  }

  /*-----------------------------------------------------------------------*/

  Mesh generate() {

    if( this->modelData.is_sampling_possible() == false){
      throw std::logic_error("Can not sample. Make sure that you are not using a restricted model!\n");
    }

    const arma::vec speakerWeights = sample_speaker_weights();
    const arma::vec phonemeWeights = sample_phoneme_weights();

    return this->reconstructMesh.for_weights(speakerWeights, phonemeWeights);

  }

  /*-----------------------------------------------------------------------*/

  Mesh generate_with_fixed_phoneme(const arma::vec& phonemeWeights) {

    arma::vec speakerWeights = sample_speaker_weights();

    return this->reconstructMesh.for_weights(speakerWeights, phonemeWeights);

  }

  /*-----------------------------------------------------------------------*/

private:

  /*-----------------------------------------------------------------------*/

  arma::vec sample_speaker_weights() {

    // sample from standard normal distribution N(0, I)
    std::normal_distribution<double> distribution(0, 1);

    arma::vec speakerWeights =
      arma::zeros(this->modelData.get_speaker_mode_dimension());

    for(double& entry: speakerWeights) {
      entry = distribution(this->generator);
    }

    // use standard normal distributed unit random vector to generate
    // a sample from the multivariate normal distribution
    // c.f., e.g., https://en.wikipedia.org/wiki/Multivariate_normal_distribution#Drawing_values_from_the_distribution
    speakerWeights =
      this->modelData.get_speaker_mean_weights() +
      this->modelData.get_speaker_sampling_matrix() * speakerWeights;

    return speakerWeights;

  }

  /*-----------------------------------------------------------------------*/

  arma::vec sample_phoneme_weights() {

    // sample from standard normal distribution N(0, I)
    std::normal_distribution<double> distribution(0, 1);

    arma::vec phonemeWeights =
      arma::zeros(this->modelData.get_phoneme_mode_dimension());

    for(double& entry: phonemeWeights) {
      entry = distribution(this->generator);
    }

    // use standard normal distributed unit random vector to generate
    // a sample from the multivariate normal distribution
    // c.f., e.g., https://en.wikipedia.org/wiki/Multivariate_normal_distribution#Drawing_values_from_the_distribution
    phonemeWeights =
      this->modelData.get_phoneme_mean_weights() +
      this->modelData.get_phoneme_sampling_matrix() * phonemeWeights;

    return phonemeWeights;

  }

  /*-----------------------------------------------------------------------*/

  const ModelData& modelData;
  const ModelMeshReconstructor& reconstructMesh;

  /*-----------------------------------------------------------------------*/

  std::default_random_engine generator;

  /*-----------------------------------------------------------------------*/
};

#endif
