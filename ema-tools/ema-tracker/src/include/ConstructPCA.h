#ifndef __CONSTRUCT_PCA_H__
#define __CONSTRUCT_PCA_H__

class ConstructPCA{

public:

  Model from(const Model& model, const arma::vec& speakerWeights) {

    // compute speaker basis vectors by computing linear combination
    // of all speakers
    const arma::mat speakerSpace =
      model.space().speaker(speakerWeights);

    // construct raw data for tensor
    std::vector<double> rawData;

    const int& phonemeDimension =
      this->trackerData.currentModel.data().get_phoneme_mode_dimension();

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
        this->trackerData.currentModel.data().get_phoneme_mean_weights());

    std::cout << this->trackerData.currentModel.data().get_phoneme_mean_weights() << std::endl;
    return Model(modelData);

  } // end compute_pca_model


private:

};

#endif
