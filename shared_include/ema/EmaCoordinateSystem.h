#ifndef __EMA_COORDINATE_SYSTEM_H__
#define __EMA_COORDINATE_SYSTEM_H__

#include <fstream>

#include <json/json.h>
#include <armadillo>

class EmaCoordinateSystem{

private:

  arma::vec left;
  arma::vec right;
  arma::vec front;

  arma::vec origin;
  arma::vec xAxis;
  arma::vec yAxis;
  arma::vec zAxis;

  arma::mat mappingMatrix;

public:

  EmaCoordinateSystem() {

  }

  EmaCoordinateSystem(
                      const arma::vec& origin,
                      const arma::vec& xAxis,
                      const arma::vec& yAxis,
                      const arma::vec& zAxis
                      ) :
    origin(origin),
    xAxis(xAxis),
    yAxis(yAxis),
    zAxis(zAxis) {

    compute_mapping_matrix();

  }

  Json::Value get_json() const {

    Json::Value root(Json::objectValue);

    root["origin"] = as_json(this->origin);
    root["xAxis"] = as_json(this->xAxis);
    root["yAxis"] = as_json(this->yAxis);
    root["zAxis"] = as_json(this->zAxis);

    return root;

  }

  void build_from(const arma::vec& left, const arma::vec& right, const arma::vec& front) {

    this->left = left;
    this->right = right;
    this->front = front;

    compute_axis();
    compute_origin();

    compute_mapping_matrix();

  }

  void build_from(Json::Value& json) {

    this->origin = read_vector(json["origin"]);
    this->xAxis = read_vector(json["xAxis"]);
    this->yAxis = read_vector(json["yAxis"]);
    this->zAxis = read_vector(json["zAxis"]);

    compute_mapping_matrix();

  }

  void read_from(const std::string& fileName) {

    std::ifstream inFile(fileName);

    Json::Value root;

    inFile >> root;

    inFile.close();

    this->origin = read_vector(root["origin"]);
    this->xAxis = read_vector(root["xAxis"]);
    this->yAxis = read_vector(root["yAxis"]);
    this->zAxis = read_vector(root["zAxis"]);

    compute_mapping_matrix();

  }

  void write_to(const std::string& fileName) const {

    std::ofstream outFile(fileName);

    Json::Value root = get_json();

    outFile << root << std::endl;

    outFile.close();

  }

  arma::vec get_origin() const {

    return this->origin;

  }

  arma::vec get_x_axis() const {

    return this->xAxis;

  }

  arma::vec get_y_axis() const {

    return this->yAxis;

  }

  arma::vec get_z_axis() const {

    return this->zAxis;

  }

  arma::vec map(const arma::vec& position) const {

    return this->mappingMatrix * ( position - this->origin );

  }

  // maps a position back to the original coordinate system
  arma::vec map_back(const arma::vec& position) const {

    return this->mappingMatrix.t() * position + this->origin;

  }

private:

  void compute_axis() {

    const arma::vec leftToRight = this->right - this->left;
    const arma::vec leftToFront = this->front - this->left;

    this->xAxis = arma::normalise(leftToRight);
    this->zAxis = arma::normalise(arma::cross(xAxis, leftToFront));
    this->yAxis = arma::normalise(arma::cross(zAxis, xAxis));

  }

  void compute_origin() {

    this->origin = (this->left + this->right + this->front) / 3;

  }

  arma::vec read_vector(const Json::Value& value) const {

    arma::vec result = arma::zeros(3);

    for(int i = 0; i < 3; ++i) {

      result(i) = value[i].asDouble();

    }

    return result;

  }

  Json::Value as_json(const arma::vec& vector) const {

    Json::Value result(Json::arrayValue);

    for(int i = 0; i < 3; ++i) {

      result.append(vector(i));

    }

    return result;

  }

  void compute_mapping_matrix() {

    this->mappingMatrix = arma::zeros(3, 3);
    this->mappingMatrix.row(0) = xAxis.t();
    this->mappingMatrix.row(1) = yAxis.t();
    this->mappingMatrix.row(2) = zAxis.t();

  }

};
#endif
