#ifndef __FIXED_CORRESPONDENCES_H__
#define __FIXED_CORRESPONDENCES_H__

#include "mesh/Mesh.h"
#include "neighborsearch/SearchProto.h"

class FixedCorrespondences : public SearchProto{

public:

  /*-------------------------------------------------------------------------*/

  FixedCorrespondences(
    Mesh& source,
    Mesh& target
    ) :
    source(source), target(target)
    {

  }

  /*-------------------------------------------------------------------------*/

  virtual void find_neighbors(
    std::vector<int>& sourceIndices,
    std::vector<int>& targetIndices
    ) const {

    sourceIndices.clear();
    targetIndices.clear();

    if(this->source.get_vertices().size() != this->target.get_vertices().size()) {
      throw std::runtime_error(
        "Cannot use fixed correspondences, meshes have different sizes.\nSource: "
        + std::to_string(this->source.get_vertices().size()) + " != Target : "
        + std::to_string(this->target.get_vertices().size())
        );
    }

    // create one to one correspondence
    for(unsigned int index = 0; index < this->source.get_vertices().size();
        ++index) {
      sourceIndices.push_back(index);
      targetIndices.push_back(index);
    }

  }

  /*-------------------------------------------------------------------------*/

private:

  /*-------------------------------------------------------------------------*/

  Mesh& source;
  Mesh& target;

  std::vector<int> indices;

  /*-------------------------------------------------------------------------*/

};
#endif
