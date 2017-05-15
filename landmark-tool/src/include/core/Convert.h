#ifndef __CONVERT_H__
#define __CONVERT_H__

#include <memory>
#include <gtkmm.h>
#include <Magick++.h>

class Convert {
  public:
    static Magick::Image* cairo_to_magick(
        Cairo::RefPtr<Cairo::ImageSurface> cairoImage
        ) {

      const int columns = cairoImage->get_width();
      const int rows = cairoImage->get_height();
      const int stride = cairoImage->get_stride();
      const int channelAmount = 4;


      // create a magick image with the same dimensions colored
      // in white
      Magick::Image *magickImage =
        new Magick::Image(Magick::Geometry(columns, rows), "white");

      // tell cairo to finish drawing on surface
      cairoImage->flush();

      // get access to raw data of cairo surface
      unsigned char* data = cairoImage->get_data();

      // iterate over the pixels
      for( int i = 0; i < columns; ++i) {
        for( int j = 0; j < rows; ++j) {
          // get data for current pixel
          const unsigned char *p = data + stride * j + i * channelAmount;

          // convert color to Magick color
          const int r = static_cast<double>(p[2]) / 255 * 65535;
          const int g = static_cast<double>(p[1]) / 255 * 65535;
          const int b = static_cast<double>(p[0]) / 255 * 65535;
          const Magick::Color currentColor(r, g, b, 0);

          // color pixel in magick image
          magickImage->pixelColor( i, j, currentColor);
        }
      }

      return magickImage;
    }

    /*----------------------------------------------------------------------*/

    static Cairo::RefPtr<Cairo::ImageSurface> magick_to_cairo(
        Magick::Image *magickImage
        ) {
      // get dimensions of image
      const int columns = magickImage->columns();
      const int rows = magickImage->rows();

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

          // get Magick color at the current pixel
          Magick::Color currentColor = magickImage->pixelColor(i, j);

          // get data for current pixels
          unsigned char *p = data + stride * j + i * channelAmount;

          // no transparency
          p[3] = 255;

          // convert color to cairo color
          p[2] = static_cast<double>(currentColor.redQuantum()) / 65535 * 255;
          p[1] = static_cast<double>(currentColor.greenQuantum()) / 65535 * 255;
          p[0] = static_cast<double>(currentColor.blueQuantum()) / 65535 * 255;
        }
      }

      // notify cairo that the surface was modified by other means than cairo
      cairoImage->mark_dirty();

      return cairoImage;

    }

    /*----------------------------------------------------------------------*/

  private:
    Convert();
};
#endif
