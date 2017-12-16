#ifndef __IMAGE_MIRROR_H__
#define __IMAGE_MIRROR_H__

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/ImageData.h"
#include "image/ImageAccess.h"

class ImageMirror{

public:

  // constructor that initializes the ImageData reference
  // and creates the ImageAccess object
  ImageMirror(ImageData& imageData) :
    imageData(imageData),
    imageAccess(ImageAccess(imageData)) {
  }

  

  

  

  

private:

  ImageData& imageData;

  

};
#endif
