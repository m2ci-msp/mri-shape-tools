#ifndef __IMAGE_STACK_BUILDER_H__
#define __IMAGE_STACK_BUILDER_H__

#include <vector>

#include "image/Image.h"
#include "core/ImageStack.h"

class ImageStackBuilder{

 public:

  std::shared_ptr<ImageStack> build_from(const Image& sourceImage) {

    this->image = sourceImage;

    // scale values to the interval [0, 255] and enhance contrast
    // discard 0.25% of darkest and brightest colors
    this->image.values().discard(0.25, 0.25);
    this->image.values().scale(0, 255);

    this->width = this->image.info().get_nx();
    this->height = this->image.info().get_ny();
    this->depth = this->image.info().get_nz();

    clear_storage();
    allocate_storage();
    create_slices();

    return std::make_shared<ImageStack>(this->xySlices, this->zxSlices, this->zySlices, this->width, this->height, this->depth);

  }

 private:

  /*----------------------------------------------------------------------*/

  std::vector<Cairo::RefPtr<Cairo::ImageSurface> > xySlices;
  std::vector<Cairo::RefPtr<Cairo::ImageSurface> > zxSlices;
  std::vector<Cairo::RefPtr<Cairo::ImageSurface> > zySlices;

  Image image;
  int width;
  int height;
  int depth;

  /*----------------------------------------------------------------------*/

  void clear_storage() {

    this->xySlices.clear();
    this->zxSlices.clear();
    this->zySlices.clear();

  }

  /*----------------------------------------------------------------------*/

  void allocate_storage() {

    for(int i = 0; i < this->depth; ++i) {

      this->xySlices.push_back( 
                               Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, this->width, this->height)
                                );

    }

    for(int i = 0; i < this->height; ++i) {

      this->zxSlices.push_back( 
                               Cairo::ImageSurface::create( Cairo::FORMAT_ARGB32, this->depth, this->width)
                                );
    }

    for(int i = 0; i < this->width; ++i) {
      this->zySlices.push_back( 
                               Cairo::ImageSurface::create( Cairo::FORMAT_ARGB32, this->depth, this->height)
                                );
    }

  }

  /*----------------------------------------------------------------------*/

  std::vector<unsigned char*> get_raw_data(
                                           std::vector<Cairo::RefPtr<Cairo::ImageSurface> > images) {

    std::vector<unsigned char*> data;

    for(auto image: images ) {
      data.push_back(image->get_data());
    }

    return data;

  }

  /*----------------------------------------------------------------------*/

  void create_slices() {

    // get information about storage mode in cairo surface
    const int strideXY = this->xySlices.at(0)->get_stride();
    const int strideZX = this->zxSlices.at(0)->get_stride();
    const int strideZY = this->zySlices.at(0)->get_stride();
    const int channelAmount = 4;

    // get raw data access to the three representations
    std::vector<unsigned char*> dataXY = get_raw_data(this->xySlices);
    std::vector<unsigned char*> dataZX = get_raw_data(this->zxSlices);
    std::vector<unsigned char*> dataZY = get_raw_data(this->zySlices);

    // now perform reslicing
    for(int z = 0; z < this->depth; ++z) {
      for(int x = 0; x < this->width; ++x) {
        for(int y = 0; y < this->height; ++y) {

          unsigned char *pXY = dataXY[z] + strideXY * y + x * channelAmount;
          unsigned char *pZX = dataZX[y] + strideZX * x + z * channelAmount;
          unsigned char *pZY = dataZY[x] + strideZY * y + z * channelAmount;

          const double& color = this->image.access().at_grid(x, y, z);

          pXY[0] = pZY[0] = pZX[0] = compute_blue_component(color);
          pXY[1] = pZY[1] = pZX[1] = compute_green_component(color);
          pXY[2] = pZY[2] = pZX[2] = compute_red_component(color);

          // no transparency
          pXY[3] = pZY[3] = pZX[3] = 255;

        } // end for y
      } // end for x
    } // end for z

    // inform each altered surface about low level data manipulation
    for(auto surface: this->xySlices) {

      surface->mark_dirty();

    }

    for(auto surface: this->zxSlices) {

      surface->mark_dirty();

    }

    for(auto surface: this->zySlices) {

      surface->mark_dirty();

    }

  }

  /*----------------------------------------------------------------------*/

 protected:

  /*----------------------------------------------------------------------*/

  virtual unsigned char compute_red_component(const double& color) const {

    return color;

  }

  /*----------------------------------------------------------------------*/

  virtual unsigned char compute_green_component(const double& color) const {

    return color;

  }

  /*----------------------------------------------------------------------*/

  virtual unsigned char compute_blue_component(const double& color) const {

    return color;

  }

  /*----------------------------------------------------------------------*/

};

#endif
