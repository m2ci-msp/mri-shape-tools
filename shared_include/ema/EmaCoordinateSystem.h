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

public:

  EmaCoordinateSystem() {

  }

  void build_from(const arma::vec& left, const arma::vec& right, const arma::vec& front) {

    this->left = left;
    this->right = right;
    this->front = front;

    compute_axis();
    compute_origin();

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

  }

  void write_to(const std::string& fileName) {

    std::ofstream outFile(fileName);

    Json::Value root(Json::objectValue);

    root["origin"] = as_json(this->origin);
    root["xAxis"] = as_json(this->xAxis);
    root["yAxis"] = as_json(this->yAxis);
    root["zAxis"] = as_json(this->zAxis);

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

  arma::vec read_vector(Json::Value& value) {

    arma::vec result = arma::zeros(3);

    for(int i = 0; i < 3; ++i) {

      result(i) = value[i].asDouble();

    }

    return result;

  }

  Json::Value as_json(const arma::vec& vector) {

    Json::Value result(Json::arrayValue);

    for(int i = 0; i < 3; ++i) {

      result.append(vector(i));

    }

    return result;

  }

};
#endif
