#ifndef __MIDSAGITTAL_ROTATION_H__
#define __MIDSAGITTAL_ROTATION_H__

#include <armadillo>

#include "mesh/Mesh.h"

class MidsagittalRotation{

private:

  Mesh pointCloud;

public:

  MidsagittalRotation(const Mesh& pointCloud) :
    pointCloud(pointCloud) {

  }

  void estimate(arma::vec& origin, double& angle) {

    angle = estimate_angle();
    origin = this->pointCloud.get_center();

    return;

  }

private:

  double estimate_angle() {

    // project point cloud into y-x-plane
    std::vector<arma::vec> points;

    for(arma::vec position: this->pointCloud.get_vertices()) {

      position(2) = 0;
      points.push_back(position);

    }

    // use PCA to find most dominant principal direction
    arma::vec dominantAxis;

    perform_pca(points, dominantAxis);

    // projected midsagittal data should vary the most in y-direction

    // compute angle in degrees between y-axis and most dominant principal direction
    // and compute angle that rotates principal direction back to y-axis
    const arma::vec yAxis({0, 1, 0});

    const double angle = - acos(arma::norm_dot(dominantAxis, yAxis)) * 180 / M_PI;

    return angle;

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
                   arma::vec& dominantAxis
                   ) const {

    const arma::vec mean = compute_mean(points);

    center_points(points, mean);

    arma::mat C = construct_covariance_matrix(points);

    arma::vec eigval;
    arma::mat eigvec;

    arma::eig_sym(eigval, eigvec, C, "std");

    dominantAxis = eigvec.col(2);

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
