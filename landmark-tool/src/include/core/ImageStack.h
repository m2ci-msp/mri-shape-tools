#ifndef __IMAGE_STACK_H__
#define __IMAGE_STACK_H__

#include <vector>

/** class reprenting an image stack */
class ImageStack {
  public:
    ImageStack(std::vector<Cairo::RefPtr<Cairo::ImageSurface> > xySlices) : 
      xySlices(xySlices) {
        // get dimensions
        this->depth = xySlices.size();
        this->width = xySlices.at(0)->get_width();
        this->height = xySlices.at(0)->get_height();

        allocate_storage_for_additional_representations();
        create_additional_representations();
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

    std::vector<unsigned char*> get_raw_data(
        std::vector<Cairo::RefPtr<Cairo::ImageSurface> > images) {

      std::vector<unsigned char*> data;

      for(auto image: images ) {
        data.push_back(image->get_data());
      }

      return data;

    }

    /*----------------------------------------------------------------------*/

    void allocate_storage_for_additional_representations() {

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

    void create_additional_representations() {

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

            // copy color information
            for( int c = 0; c < channelAmount; ++c) {
              pZX[c] = pXY[c];
              pZY[c] = pXY[c];
            } // end for c

          } // end for y
        } // end for x
      } // end for z

      // inform each altered surface about low level data manipulation
      for(auto surface: this->zxSlices) {
        surface->mark_dirty();
      }

      for(auto surface: this->zySlices) {
        surface->mark_dirty();
      }

    }

    /*----------------------------------------------------------------------*/

    std::vector<Cairo::RefPtr<Cairo::ImageSurface> > xySlices;
    std::vector<Cairo::RefPtr<Cairo::ImageSurface> > zxSlices;
    std::vector<Cairo::RefPtr<Cairo::ImageSurface> > zySlices;

    int width;
    int height;
    int depth;

};
#endif
