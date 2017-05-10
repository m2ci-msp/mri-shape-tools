#ifndef __TENSOR_DATA_H__
#define __TENSOR_DATA_H__

#include <vector>

class TensorData{

public:

  /*---------------------------------------------------------------------------*/

  TensorData() {

    this->modeOneDimension = 0;
    this->modeTwoDimension = 0;
    this->modeThreeDimension = 0;

  }

  /*---------------------------------------------------------------------------*/

  TensorData& set_data( const std::vector<double>& data ) {

    this->data = data;

    return *this;

  }

  /*---------------------------------------------------------------------------*/

  TensorData& set_mode_dimensions(
    const int& modeOne,
    const int& modeTwo,
    const int& modeThree
    ) {

    this->modeOneDimension = modeOne;
    this->modeTwoDimension = modeTwo;
    this->modeThreeDimension = modeThree;;

    return *this;
  }

  /*---------------------------------------------------------------------------*/

  int get_mode_one_dimension() const {
    return this->modeOneDimension;
  }

  /*---------------------------------------------------------------------------*/

  int get_mode_two_dimension() const {
    return this->modeTwoDimension;
  }

  /*---------------------------------------------------------------------------*/

  int get_mode_three_dimension() const {
    return this->modeThreeDimension;
  }

  /*---------------------------------------------------------------------------*/

  std::vector<double>& get_data() {
    return this->data;
  }

  /*---------------------------------------------------------------------------*/

  const std::vector<double>& get_data() const {
    return this->data;
  }

  /*---------------------------------------------------------------------------*/


private:

  /*---------------------------------------------------------------------------*/

  int modeOneDimension;
  int modeTwoDimension;
  int modeThreeDimension;

  std::vector<double> data;

  /*---------------------------------------------------------------------------*/

};

#endif
