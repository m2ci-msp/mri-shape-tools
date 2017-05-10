#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#include <vector>

#include <armadillo>

class Serializer{

public:

  /*--------------------------------------------------------------------------*/

  static arma::vec serialize(const std::vector<arma::vec>& points) {

    arma::vec result = arma::zeros(points.size() * 3);

    for(size_t i = 0; i < points.size(); ++i) {

      const arma::vec point = points.at(i);

      result(3 * i + 0) = point(0);
      result(3 * i + 1) = point(1);
      result(3 * i + 2) = point(2);

    }

    return result;

  }

  /*--------------------------------------------------------------------------*/

  static std::vector<arma::vec> unserialize(const arma::vec& serialized) {

    const size_t amount = serialized.n_elem / 3;
    std::vector<arma::vec> result;

    for(size_t i = 0; i < amount; ++i) {

      arma::vec point = arma::zeros(3);

      point(0) = serialized(i * 3 + 0);
      point(1) = serialized(i * 3 + 1);
      point(2) = serialized(i * 3 + 2);

      result.push_back(point);

    }

    return result;

  }

  /*--------------------------------------------------------------------------*/

};
#endif
