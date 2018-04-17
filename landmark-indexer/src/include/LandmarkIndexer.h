#ifndef __LANDMARK_INDEXER_H__
#define __LANDMARK_INDEXER_H__

#include "alignment/KdTree.h"
#include "mesh/Mesh.h"

class LandmarkIndexer {

  public:

    LandmarkIndexer(const Mesh& mesh) : kdTree(mesh.get_vertices()) {
    }

    void compute_indices(std::vector<Landmark>& landmarks) {

      for(auto& mark: landmarks) {

        const int index = kdTree.get_nearest_neighbor_index(mark.sourcePosition);
        mark.sourceIndex = index;

      }

    }
    
  private:

    KdTree kdTree;
  
};

#endif
