#ifndef __EMA_POINT_CLOUD_H__
#define __EMA_POINT_CLOUD_H__

#include "mesh/Mesh.h"
#include "ema/EmaData.h"

class EmaPointCloud{

 private:

  const EmaData& emaData;

 public:

  EmaPointCloud(const EmaData& emaData) : emaData(emaData) {

  }

  Mesh from(const std::vector<std::string>& coilIds, const int& sampleIndex) const {

    std::vector<arma::vec> vertices;

    for(const std::string& id: coilIds) {

      const EmaCoil& coil = this->emaData.emaCoils.at(id);
      vertices.push_back(coil.access().position(sampleIndex));

    }

    Mesh mesh;
    mesh.set_vertices(vertices);

    return mesh;

  }

};
#endif
