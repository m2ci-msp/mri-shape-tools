#ifndef __TENSOR_ACCESS_H__
#define __TENSOR_ACCESS_H__

#include "tensor/TensorData.h"

class TensorAccess{

public:

  /*---------------------------------------------------------------------------*/

  TensorAccess(TensorData& data) : tensorData(data) {
  }

  /*---------------------------------------------------------------------------*/

  double get_entry(
    const int& modeOne,
    const int& modeTwo,
    const int& modeThree
    ) const {

    const int& modeTwoDimension = tensorData.get_mode_two_dimension();
    const int& modeThreeDimension = tensorData.get_mode_three_dimension();

    const int index =
      modeOne * modeTwoDimension * modeThreeDimension +
      modeTwo * modeThreeDimension +
      modeThree;

    return tensorData.get_data().at(index);

  }

  /*---------------------------------------------------------------------------*/

private:

  /*---------------------------------------------------------------------------*/

  TensorData& tensorData;

  /*---------------------------------------------------------------------------*/
};

#endif
