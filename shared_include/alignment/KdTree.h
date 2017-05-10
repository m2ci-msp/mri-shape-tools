#ifndef __KD_TREE_H__
#define __KD_TREE_H__

#include <vector>
#include <armadillo>

#include <ANN/ANN.h>

class KdTree{

  public:

    /*----------------------------------------------------------------------------*/

    KdTree() {
      this->kdTree = nullptr;
    }

    /*----------------------------------------------------------------------------*/

    KdTree(
        const std::vector<arma::vec>& points
        ) {

      this->dataPts = annAllocPts(points.size(), 3);

      for( size_t index = 0; index < points.size(); ++index) {

        this->dataPts[index][0] = points.at(index)(0);
        this->dataPts[index][1] = points.at(index)(1);
        this->dataPts[index][2] = points.at(index)(2);


      }

      this->kdTree = new ANNkd_tree(this->dataPts, points.size(), 3);
      this->points = points;

    }

    /*----------------------------------------------------------------------------*/

    ~KdTree() {
      if( this->kdTree != nullptr ) {
        annDeallocPts(dataPts);
        delete kdTree;
        this->kdTree = nullptr;
      }
    }

    /*----------------------------------------------------------------------------*/

    arma::vec get_nearest_neighbor(const arma::vec& point) const {

      const int index = get_nearest_neighbor_index(point);
      return this->points.at(index);
    }
    /*----------------------------------------------------------------------------*/

    int get_nearest_neighbor_index(const arma::vec& point) const {

      ANNpoint queryPt;
      ANNidxArray nnIdx;
      ANNdistArray dists;

      queryPt = annAllocPt(3);
      nnIdx = new ANNidx[1];
      dists = new ANNdist[1];

      nnIdx[0] = -1;

      queryPt[0] = point(0);
      queryPt[1] = point(1);
      queryPt[2] = point(2);

      kdTree->annkPriSearch(queryPt, 1, nnIdx, dists);

      const int index = nnIdx[0];

      annDeallocPt(queryPt);
      delete [] nnIdx;
      delete [] dists;

      return index;

    }

    /*----------------------------------------------------------------------------*/

    std::vector<arma::vec> get_nearest_neighbors(
      const arma::vec& point,
      const double radius) const {

      ANNpoint queryPt;
      ANNidxArray nnIdx;
      ANNdistArray dists;

      const double squaredRadius = pow(radius, 2);

      queryPt = annAllocPt(3);

      queryPt[0] = point(0);
      queryPt[1] = point(1);
      queryPt[2] = point(2);

      unsigned int k = kdTree->annkFRSearch(queryPt, squaredRadius, 0, NULL, NULL);

      nnIdx = new ANNidx[k];
      dists = new ANNdist[k];

      kdTree->annkFRSearch(queryPt, squaredRadius, k, nnIdx, dists);

      std::vector<arma::vec> result;

      for(size_t i = 0; i < k; ++i) {
        result.push_back(this->points.at(nnIdx[i]));
      }

      annDeallocPt(queryPt);
      delete [] nnIdx;
      delete [] dists;

      return result;

    }

    /*----------------------------------------------------------------------------*/

    std::vector<int> get_nearest_neighbors_index(
      const arma::vec& point,
      const double radius) const {

      ANNpoint queryPt;
      ANNidxArray nnIdx;
      ANNdistArray dists;

      const double squaredRadius = pow(radius, 2);

      queryPt = annAllocPt(3);

      queryPt[0] = point(0);
      queryPt[1] = point(1);
      queryPt[2] = point(2);

      unsigned int k = kdTree->annkFRSearch(queryPt, squaredRadius, 0, NULL, NULL);

      nnIdx = new ANNidx[k];
      dists = new ANNdist[k];

      kdTree->annkFRSearch(queryPt, squaredRadius, k, nnIdx, dists);

      std::vector<int> result;

      for(size_t i = 0; i < k; ++i) {
        result.push_back(nnIdx[i]);
      }

      annDeallocPt(queryPt);
      delete [] nnIdx;
      delete [] dists;

      return result;

    }

    /*----------------------------------------------------------------------------*/


  private:

    /*----------------------------------------------------------------------------*/

    ANNpointArray dataPts;
    ANNkd_tree* kdTree;
    std::vector<arma::vec> points;

    /*----------------------------------------------------------------------------*/

};
#endif
