#ifndef __EMA_COIL_POSITIONS_BUILDER_H__
#define __EMA_COIL_POSITIONS_BUILDER_H__

#include <vector>
#include <armadillo>
#include "EmaCoilPositions.h"

class EmaCoilPositionsBuilder{

public:

    static EmaCoilPositions build(const std::vector<double>& serialized) {
  
      // perform integrity check
      if( serialized.size() % 3 != 0 ) {
  
        throw std::runtime_error("Data size mismatch: Positional data samples array size is no mutliple of 3.")
  
        }
  
  size_t sampleAmount = serialized.size() / 3;
  
      std::vector<arma::vec> positions;
  
      // iterate through data samples and reconstruct coordinates
      for(size_t i = 0; i < sampleAmount; ++i) {
  
        const double x = serialized[3 * i + 0];
        const double y = serialized[3 * i + 1];
        const double z = serialized[3 * i + 2];
      
        positions.push_back( {x, y, z} );
      
      }
  
      return EmaCoilPositions(positions);
  
    }

private:

  EmaCoilPositionsBuilder();

};
#endif
