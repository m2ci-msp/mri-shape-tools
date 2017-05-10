#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "image/ImageData.h"
#include "image/ImageAnalysis.h"
#include "image/ImageSmooth.h"

class Image{

public:

  /*----------------------------------------------------------------------*/

  Image() {
    this->valid = false;
  }

  /*----------------------------------------------------------------------*/

  Image(const ImageData& imageData) : imageData(imageData) {

    this->imageSmooth = new ImageSmooth(this->imageData);
    this->imageAnalysis = new ImageAnalysis(this->imageData);
    this->valid = true;

  }

  /*----------------------------------------------------------------------*/

  Image(const Image& image) : imageData(image.imageData) {

    cleanup();

    this->imageSmooth = new ImageSmooth(this->imageData);
    this->imageAnalysis = new ImageAnalysis(this->imageData);
    this->valid = true;

  }

  /*----------------------------------------------------------------------*/

  Image& operator=( const Image& other) {

    cleanup();

    this->imageData = other.imageData;
    this->imageSmooth = new ImageSmooth(this->imageData);
    this->imageAnalysis = new ImageAnalysis(this->imageData);

    this->valid = true;

    return *this;

  }

  /*----------------------------------------------------------------------*/

  ~Image() {

    cleanup();

  }

  /*----------------------------------------------------------------------*/

  ImageData& data() {

    return this->imageData;

  }

  /*----------------------------------------------------------------------*/

  const ImageData& data() const {

    return this->imageData;

  }

  /*----------------------------------------------------------------------*/


  ImageSmooth& smooth() {

    return *(this->imageSmooth);

  }

  /*----------------------------------------------------------------------*/

  ImageAnalysis& analysis() {

    return *(this->imageAnalysis);

  }

  /*----------------------------------------------------------------------*/

private:

  /*----------------------------------------------------------------------*/

  void cleanup() {

    if( this->valid == true ) {
      delete this->imageSmooth;
      delete this->imageAnalysis;
    }

  }

  /*----------------------------------------------------------------------*/

  ImageData imageData;
  ImageSmooth* imageSmooth;
  ImageAnalysis* imageAnalysis;

  /*----------------------------------------------------------------------*/

  bool valid = false;

  /*----------------------------------------------------------------------*/

};


#endif
