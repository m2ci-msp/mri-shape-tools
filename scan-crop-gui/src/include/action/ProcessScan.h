#ifndef __PROCESS_SCAN_H__
#define __PROCESS_SCAN_H__

#include "action/Action.h"
#include "core/ImageStack.h"

#include "scan/Scan.h"
#include "scan/ScanIO.h"

/** class that processes a scan */
class ProcessScan : public Action {

  public:
    ProcessScan(const std::string& path) :
      scan(ScanIO::read_from(path)) {
    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      // save original scan
      Data::get_instance()->set_scan(this->scan);

      // enhance contrast
      this->scan.transform()->discard_values(0.25, 0.25);

      // scale scan values to the interval [0, 255]
      this->scan.transform()->scale_values(0, 255);
      const size_t voxelAmount = scan.data()->get_values().size();

      std::vector<double> values;
      values.resize(voxelAmount, 0);

      Data::get_instance()->set_image_stack(get_image_stack());

    }

    /*----------------------------------------------------------------------*/

  private:

    /*----------------------------------------------------------------------*/

    Scan scan;

    /*----------------------------------------------------------------------*/

    std::shared_ptr<ImageStack>
      get_image_stack() {

        std::vector<Cairo::RefPtr<Cairo::ImageSurface> > slices;

        for(int i = 0; i < this->scan.data()->get_nz(); ++i) {
          slices.push_back( create_image_from_slice(i) );
        }

        std::shared_ptr<ImageStack> stack =
          std::make_shared<ImageStack>(slices);


        return stack;

      }


    /*----------------------------------------------------------------------*/

    Cairo::RefPtr<Cairo::ImageSurface>
      create_image_from_slice( const int& slice ) {

      // get dimensions of image
      const int columns = this->scan.data()->get_nx();
      const int rows = this->scan.data()->get_ny();

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
          double currentColor = this->scan.access()->get_value_index(i, j, slice);

          // get data for current pixels
          unsigned char *p = data + stride * j + i * channelAmount;

          // no transparency
          p[3] = 255;

          // set color
          p[2] = currentColor;
          p[1] = currentColor;
          p[0] = currentColor;
        }
      }

      // notify cairo that the surface was modified by other means than cairo
      cairoImage->mark_dirty();

      return cairoImage;


    }

    /*----------------------------------------------------------------------*/
};
#endif
