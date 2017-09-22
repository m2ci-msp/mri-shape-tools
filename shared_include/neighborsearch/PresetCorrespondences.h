#ifndef __PRESET_CORRESPONDENCES_H__
#define __PRESET_CORRESPONDENCES_H__

#include <vector>

#include "neighborsearch/SearchProto.h"

class PresetCorrespondences : public SearchProto{

public:

  /*-------------------------------------------------------------------------*/

  PresetCorrespondences(
    std::vector<int>& presetSource,
    std::vector<int>& presetTarget
    ) :
    presetSource(presetSource), presetTarget(presetTarget)
    {

  }

  /*-------------------------------------------------------------------------*/

  virtual void find_neighbors(
    std::vector<int>& sourceIndices,
    std::vector<int>& targetIndices
    ) const {

    sourceIndices.clear();
    targetIndices.clear();

    if(this->presetSource.size() != this->presetTarget.size()) {
      throw std::runtime_error(
        "Cannot use preset correspondences, size mismatch.\nSource: "
        + std::to_string(this->presetSource.size()) + " != Target : "
        + std::to_string(this->presetTarget.size())
        );
    }

    for(unsigned int index = 0; index < this->presetSource.size(); ++index) {

      sourceIndices.push_back(this->presetSource[index]);
      targetIndices.push_back(this->presetTarget[index]);

    }

  }

  /*-------------------------------------------------------------------------*/

private:

  /*-------------------------------------------------------------------------*/

  std::vector<int>& presetSource;
  std::vector<int>& presetTarget;

  /*-------------------------------------------------------------------------*/

};
#endif
