#ifndef __EMA_COIL_POSITIONS_H__
#define __EMA_COIL_POSITIONS_H__

#include "mesh/Mesh.h"
#include "ema/EmaCoilData.h"

class EmaCoilPositions{

private:

  const EmaCoilData& emaCoilData;

public:

  EmaCoilPositions(const EmaCoilData& emaCoilData) : emaCoilData(emaCoilData) {

  }

  Mesh as_point_cloud() const {

    Mesh mesh;
    mesh.set_vertices(this->emaCoilData.positions);

    return mesh;

  }

};
#endif
