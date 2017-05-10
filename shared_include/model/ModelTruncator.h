#ifndef __MODEL_TRUNCATOR_H__
#define __MODEL_TRUNCATOR_H__

class ModelTruncator{

public:

  /*--------------------------------------------------------------------------*/

  ModelTruncator(ModelData& modelData, ModelSpace& modelSpace) :
    modelData(modelData), modelSpace(modelSpace) {
  }

  /*--------------------------------------------------------------------------*/

  /*
  void speaker(const std::set<int>& indicesSpeaker) {

    std::vector<double> truncatedMeanWeights;

    for(const int& index: indicesSpeaker) {

      truncatedMeanWeights.push_back(
        this->modelData.get_speaker_mean_weights().at(index)
        );

    }

    this->modelData.get_core_tensor().truncate().mode_one(indicesSpeaker);
    this->modelData.set_speaker_mean_weights(truncatedMeanWeights);

    this->modelSpace.update();

  }
  */

  /*--------------------------------------------------------------------------*/

  /*
  void phoneme(const std::set<int>& indicesPhoneme) {

    std::vector<double> truncatedMeanWeights;

    for(const int& index: indicesPhoneme) {

      truncatedMeanWeights.push_back(
        this->modelData.get_phoneme_mean_weights().at(index)
        );

    }

    this->modelData.get_core_tensor().truncate().mode_two(indicesPhoneme);
    this->modelData.set_phoneme_mean_weights(truncatedMeanWeights);

    this->modelSpace.update();

  }
  */

  /*--------------------------------------------------------------------------*/

  /*
  // convenience version of speaker() that removes the last component
  void decrement_speaker() {

    std::set<int> indices;

    for(int i = 0; i < this->modelData.get_speaker_mode_dimension() - 1; ++i) {
      indices.insert(i);
    }

    speaker(indices);

  }
  */

  /*--------------------------------------------------------------------------*/

  /*
  // convenience version of phoneme() that removes the last component
  void decrement_phoneme() {

    std::set<int> indices;

    for(int i = 0; i < this->modelData.get_phoneme_mode_dimension() - 1; ++i) {
      indices.insert(i);
    }

    phoneme(indices);

  }
  */

  /*--------------------------------------------------------------------------*/

  void vertex(const std::set<int>& indicesVertex) {

    arma::vec truncatedShapeSpaceOrigin = arma::zeros(indicesVertex.size());

    int i = 0;

    for(const int& index: indicesVertex) {

      truncatedShapeSpaceOrigin(i) =
        this->modelData.get_shape_space_origin()(index);

      ++i;

    }

    this->modelData.get_core_tensor().truncate().mode_three(indicesVertex);
    this->modelData.set_shape_space_origin(truncatedShapeSpaceOrigin);

    this->modelData.mark_as_truncated();

    this->modelSpace.update();

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  ModelData& modelData;
  ModelSpace& modelSpace;

  /*--------------------------------------------------------------------------*/

};

#endif
