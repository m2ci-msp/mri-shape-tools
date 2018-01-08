#ifndef __OTSU_H__
  #define __OTSU_H__

  #include "image/ImageData.h"
  #include "image/ImageBoundary.h"
  #include "image/ImageValues.h"
  #include "image/segmentation/Threshold.h"
  #include "image/segmentation/otsu/OtsuEnergy.h"

  class Otsu{

  public:

    Otsu(ImageData& imageData) : imageData(imageData), imageCopy(imageData) {

        // remove boundary in copy
        ImageBoundary(imageCopy).change(0, 0, 0);

    }

    void apply() {

      ImageValues imageValues(this->imageCopy);
      imageValues.scale(0, 255);
      imageValues.round();

      OtsuEnergy energy(this->imageCopy);

      double minEnergy = DBL_MAX;
      double bestThreshold = 0;

      for(int t = 1; t < 255; ++t) {

        const double currentEnergy = energy.get_energy(t);
        if( currentEnergy < minEnergy) {
          minEnergy = currentEnergy;
          bestThreshold = t;
        }

      }

      Threshold(this->imageCopy, bestThreshold ).apply();

      create_result();

    }

  private:

void create_result() {

      const int& nx = this->imageData.nx;
      const int& ny = this->imageData.ny;
      const int& nz = this->imageData.nz;

      ImageAccess imageAccess(this->imageData);
      ImageAccess segmentationAccess(this->imageCopy);

      for(int i = 0; i < nx; ++i) {
        for(int j = 0; j < ny; ++j) {
          for(int k = 0; k < nz; ++k) {

            imageAccess.at_grid(i, j, k) = segmentationAccess.at_grid(i, j, k);

          }
        }
      }

    }

    ImageData& imageData;

    ImageData imageCopy;

  };

  #endif
