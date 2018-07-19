#ifndef __AXIS_ANALYSIS_H__
#define __AXIS_ANALYSIS_H__

#include <armadillo>

#include "ema/EmaCoordinateSystem.h"

#include "mesh/Mesh.h"

class AxisAnalysis{

private:

  Mesh pointCloud;

  arma::vec xAxis;
  arma::vec yAxis;
  arma::vec zAxis;

public:

  AxisAnalysis(const Mesh& pointCloud) :
    pointCloud(pointCloud) {

  }

  EmaCoordinateSystem get_data_aligned_coordinate_system() {

    analyze_in_x_y_plane();

    return EmaCoordinateSystem(
                               this->pointCloud.get_center(),
                               this->xAxis,
                               this->yAxis,
                               this->zAxis
                               );

  }

private:

  void analyze_in_x_y_plane() {

    // project point cloud into y-x-plane
    std::vector<arma::vec> points;

    for(arma::vec position: this->pointCloud.get_vertices()) {

      position(2) = 0;
      points.push_back(position);

    }

    // use PCA to find a data aligned coordinate system
    arma::vec firstAxis, secondAxis, thirdAxis;

    perform_pca(points, firstAxis, secondAxis, thirdAxis);

    // projected midsagittal data should vary the most in y-direction
    this->yAxis = firstAxis;

    // next dominating direction should be x
    this->xAxis = secondAxis;

    // z-Axis is the last one -> no variance because of projection
    this->zAxis = thirdAxis;

  }

  arma::vec compute_mean(const std::vector<arma::vec>& points) const {

    arma::vec mean = arma::zeros(3);

    for(const arma::vec& point: points) {

      mean += point;

    }

    return mean / points.size();

  }

  void center_points(std::vector<arma::vec>& points, const arma::vec& mean) const {

    for(arma::vec& point: points) {

      point -= mean;

    }

  }

  void perform_pca(
                   std::vector<arma::vec> points,
                   arma::vec& firstAxis,
                   arma::vec& secondAxis,
                   arma::vec& thirdAxis
                   ) const {

    const arma::vec mean = compute_mean(points);

    center_points(points, mean);

    arma::mat C = construct_covariance_matrix(points);

    arma::vec eigval;
    arma::mat eigvec;

    arma::eig_sym(eigval, eigvec, C, "std");

    firstAxis = eigvec.col(2);
    secondAxis = eigvec.col(1);
    thirdAxis = eigvec.col(0);

  }

  arma::mat construct_covariance_matrix(const std::vector<arma::vec>& points) const {

    arma::mat result = arma::zeros(3, 3);

    for(const arma::vec point: points) {

      result += point * point.t();

    }

    return result;

  }

};
#endif
