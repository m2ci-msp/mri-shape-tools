#ifndef __TRACKER_UPDATE_H__
#define __TRACKER_UPDATE_H__

#include "TrackerData.h"
#include "TrackerState.h"
#include "TrackerFitting.h"

class TrackerUpdate{

public:

  /*--------------------------------------------------------------------------*/

  TrackerUpdate(
    TrackerData& trackerData,
    TrackerState& trackerState,
    TrackerFitting& trackerFitting
    ) :
    trackerData(trackerData),
    trackerState(trackerState),
    trackerFitting(trackerFitting) {

  }

  /*--------------------------------------------------------------------------*/

  void for_source_ids() {

    // create copy of model
    this->trackerData.currentModel =
      this->trackerData.originalModel;

    // generate list of entry indices
    std::set<int> vertexIndices;

    for(const int& id: this->trackerData.sourceIds) {

      for(int i = 0; i < 3; ++i) {
        vertexIndices.insert(3 * id + i);
      }

    }

    // truncate the model
    this->trackerData.currentModel.truncate().vertex(vertexIndices);

    this->trackerState.sourceIdsSet = true;

  }

  /*--------------------------------------------------------------------------*/

  void to_fixed_speaker(const arma::vec& speakerWeights) {

    this->trackerData.currentModel = compute_pca_model(speakerWeights);
    this->trackerData.fixedSpeakerWeights = speakerWeights;

    // reinitialize the fitting data structures
    this->trackerFitting.reinit();

    this->trackerState.isPCA = true;

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  Model compute_pca_model(const arma::vec& speakerWeights) {

    // compute speaker basis vectors by computing linear combination
    // of all speakers
    const arma::mat speakerSpace =
      this->trackerData.currentModel.space().speaker(speakerWeights);

    // construct raw data for tensor
    std::vector<double> rawData;

    const int& phonemeDimension =
      this->trackerData.currentModel.data().get_phoneme_mode_dimension();

    const int& originalPhonemeDimension =
      this->trackerData.currentModel.data().get_original_phoneme_mode_dimension();

    const int& vertexDimension =
      this->trackerData.currentModel.data().get_vertex_mode_dimension();

    for(int j = 0; j < phonemeDimension; ++j) {
      for(int i = 0; i < vertexDimension; ++i) {
        rawData.push_back(speakerSpace(i, j));
      }
    }

    // construct tensor
    // speaker mode dimension is now 1
    TensorData tensorData;
    tensorData\
      .set_data(rawData)                        \
      .set_mode_dimensions(1, phonemeDimension, vertexDimension);

    Tensor tensor(tensorData);

    // construct model
    ModelData modelData;

    modelData                                   \
      .set_core_tensor(tensor)                  \
      .set_shape_space_origin(
        this->trackerData.currentModel.data().get_shape_space_origin()
        )                                       \
      // mean weight for speaker is now 1
      .set_speaker_mean_weights(arma::vec({1})) \
      .set_phoneme_mean_weights(
        this->trackerData.currentModel.data().get_phoneme_mean_weights()) \
      .set_original_speaker_mode_dimension(1) \
      .set_original_phoneme_mode_dimension(originalPhonemeDimension);

    return Model(modelData);

  } // end compute_pca_model

  /*--------------------------------------------------------------------------*/

  TrackerData& trackerData;
  TrackerState& trackerState;
  TrackerFitting& trackerFitting;

  /*--------------------------------------------------------------------------*/

};

#endif
