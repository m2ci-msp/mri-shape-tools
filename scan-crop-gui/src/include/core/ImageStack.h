#ifndef __IMAGE_STACK_H__
#define __IMAGE_STACK_H__

#include <vector>

/** class reprenting an image stack */
class ImageStack {
 public:
 ImageStack(
            std::vector<Cairo::RefPtr<Cairo::ImageSurface> >& xySlices,
            std::vector<Cairo::RefPtr<Cairo::ImageSurface> >& zxSlices,
            std::vector<Cairo::RefPtr<Cairo::ImageSurface> >& zySlices,
            const int& width,
            const int& height,
            const int& depth
            ) :
  xySlices(xySlices),
    zxSlices(zxSlices),
    zySlices(zySlices),
    width(width),
    height(height),
    depth(depth) {
    }

  /*----------------------------------------------------------------------*/

  const std::vector<Cairo::RefPtr<Cairo::ImageSurface>>& 
    get_xy_slices() const {
    return this->xySlices;
  }

  /*----------------------------------------------------------------------*/

  const std::vector<Cairo::RefPtr<Cairo::ImageSurface>>& 
    get_zx_slices() const {
    return this->zxSlices;
  }

  /*----------------------------------------------------------------------*/

  const std::vector<Cairo::RefPtr<Cairo::ImageSurface>>& 
    get_zy_slices() const {
    return this->zySlices;
  }

  /*----------------------------------------------------------------------*/

  int get_width() const {
    return this->width;
  }

  /*----------------------------------------------------------------------*/

  int get_height() const {
    return this->height;
  }

  /*----------------------------------------------------------------------*/

  int get_depth() const {
    return this->depth;
  }

  /*----------------------------------------------------------------------*/

 private:

  ImageStack();

  /*----------------------------------------------------------------------*/

  std::vector<Cairo::RefPtr<Cairo::ImageSurface> > xySlices;
  std::vector<Cairo::RefPtr<Cairo::ImageSurface> > zxSlices;
  std::vector<Cairo::RefPtr<Cairo::ImageSurface> > zySlices;

  int width;
  int height;
  int depth;

  /*----------------------------------------------------------------------*/

};
#endif
