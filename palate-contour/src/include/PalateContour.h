#ifndef __PALATE_CONTOUR_H__
#define __PALATE_CONTOUR_H__

#include <vector>
#include <armadillo>

#include "alignment/KdTree.h"
#include "utility/AxisAccess.h"

class PalateContour{

public:

  PalateContour(const std::vector<arma::vec>& points) :
    originalPoints(points),
    // use standard access
    axisAccess("XYZ") {

    project_points();

  }

  // constructor that also takes a axis access specification
  PalateContour(
		const std::vector<arma::vec>& points,
		const std::string& specification
		) :
    originalPoints(points),
    axisAccess(specification) {

    project_points();

  }

  ~PalateContour() {
    delete this->projectedPointsTree;
  }

  // returns the boundary points of the underlying point cloud
  std::vector<arma::vec> get_boundary(const double& radius, const int& samplesX, const int& samplesY) {

    std::vector<arma::vec> boundary;

    // create sample points on grid
    std::vector<arma::vec> samplePoints = generate_sample_points(samplesX, samplesY);

    for(arma::vec point : samplePoints) {

      std::vector<int> currentNeighborhood = get_nearest_neighbors(point, radius);

      if(currentNeighborhood.size() == 0) { continue; }

      // use computed height for sample point
      point( this->axisAccess.z() ) = get_boundary_height(currentNeighborhood);

      // and add to boundary
      boundary.push_back(point);

    }

    return boundary;

  }

private:

  std::vector<arma::vec> generate_sample_points(const int& samplesX, const int& samplesY) {

    // determine spacings between sample points
    const double spacingX = ( this->maxX - this->minX ) / (samplesX + 1);
    const double spacingY = ( this->maxY - this->minY ) / (samplesY + 1);

    // generate points on grid
    std::vector<arma::vec> samplePoints;

    for(int i = 0; i < samplesX + 1; ++i) {

      const double x = this->minX + spacingX * i;

      for(int j = 0; j < samplesY + 1; ++j) {

	const double y = this->minY + spacingY * j;

	const arma::vec samplePoint({x, y, 0});
	samplePoints.push_back(samplePoint);

      }
    }

    return samplePoints;

  }

  // returns the boundary height from a set of indices
  double get_boundary_height(const std::vector<int>&  points) const {

    double max = - DBL_MAX;

    for(const int& index: points) {

      const arma::vec point = this->originalPoints.at(index);

      if(point( this->axisAccess.z() ) > max) {
	max = point( this->axisAccess.z() );
      }

    }

    return max;

  }

  // project points onto x-y plane and build kd tree
  void project_points() {

    // reset bounding box
    this->minX = DBL_MAX;
    this->minY = DBL_MAX;

    this->maxX = -DBL_MAX;
    this->maxY = -DBL_MAX;

    this->projectedPoints.clear();

    for(arma::vec point: this->originalPoints) {

      this->minX = (point( this->axisAccess.x() ) < this->minX)? point( this->axisAccess.x() ) : this->minX;
      this->minY = (point( this->axisAccess.y() ) < this->minY)? point( this->axisAccess.y() ) : this->minY;

      this->maxX = (point( this->axisAccess.x() ) > this->maxX)? point( this->axisAccess.x() ) : this->maxX;
      this->maxY = (point( this->axisAccess.y() ) > this->maxY)? point( this->axisAccess.y() ) : this->maxY;

      point( this->axisAccess.z() ) = 0;
      this->projectedPoints.push_back(point);

    }

    this->projectedPointsTree = new KdTree(this->projectedPoints);

  }

  // get nearest neighbors in projection circle
  std::vector<int> get_nearest_neighbors(const arma::vec& point, const int& radius) {

    return this->projectedPointsTree->get_nearest_neighbors_index(point, radius);

  }

  std::vector<arma::vec> projectedPoints;
  std::vector<arma::vec> originalPoints;

  const AxisAccess axisAccess;

  KdTree* projectedPointsTree;

  double minX;
  double minY;

  double maxX;
  double maxY;

};
#endif
