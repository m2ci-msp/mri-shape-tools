#ifndef __PROCESS_SEGMENTATION_H__
#define __PROCESS_SEGMENTATION_H__

#include "action/ProcessScan.h"
#include "core/ImageStack.h"

#include "image/Image.h"

class ProcessSegmentation : public ProcessScan {

  public:

    ProcessSegmentation(const std::string& path) : ProcessScan(path) {

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      Data::get_instance()->set_segmentation_stack(get_image_stack());

    }

    /*----------------------------------------------------------------------*/

    virtual Cairo::RefPtr<Cairo::ImageSurface>
      create_image_from_slice( const int& slice ) {

      // get dimensions of image
      const int columns = this->scan.info().get_nx();
      const int rows = this->scan.info().get_ny();

      // create new cairo image surface
      Cairo::RefPtr<Cairo::ImageSurface> cairoImage = 
        Cairo::ImageSurface::create( Cairo::FORMAT_ARGB32, columns, rows);

      const int stride = cairoImage->get_stride();
      const int channelAmount = 4;

      // get access to raw data of cairo surface
      unsigned char* data = cairoImage->get_data();

      // iterate over the pixels
      for( int i = 0; i < columns; ++i) {
        for( int j = 0; j < rows; ++j) {

          // get color at the current voxel
          double currentColor = this->scan.access().at_grid(i, j, slice);

          // get data for current pixels
          unsigned char *p = data + stride * j + i * channelAmount;

          // no transparency
          p[3] = 255;

          // set color
          p[2] = (currentColor == 255)? 255: 0;
          p[1] = 0;
          p[0] = (currentColor == 0)? 255: 0;
        }
      }

      // notify cairo that the surface was modified by other means than cairo
      cairoImage->mark_dirty();

      return cairoImage;


    }

    /*----------------------------------------------------------------------*/

};
#endif
