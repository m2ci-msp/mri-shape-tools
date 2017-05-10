#ifndef __IMAGE_ANALYSIS_H__
#define __IMAGE_ANALYSIS_H__

#include "image/ImageData.h"

class ImageAnalysis{

public:

  /*----------------------------------------------------------------------*/

  ImageAnalysis(const ImageData& data) : data(data) {
  }

  /*----------------------------------------------------------------------*/

  double derivative_x(const int& x, const int& y, const int& z) const {

    return
      this->data.at(x + 1, y, z) -
      this->data.at(x    , y, z);

  }

  /*----------------------------------------------------------------------*/

  double derivative_y(const int& x, const int& y, const int& z) const {

    return
      this->data.at(x, y + 1, z) -
      this->data.at(x    , y, z);

  }

  /*----------------------------------------------------------------------*/

  double derivative_z(const int& x, const int& y, const int& z) const {

    return
      this->data.at(x, y, z + 1) -
      this->data.at(x    , y, z);

  }

  /*----------------------------------------------------------------------*/


private:

  /*----------------------------------------------------------------------*/

  const ImageData& data;

  /*----------------------------------------------------------------------*/

};

#endif
