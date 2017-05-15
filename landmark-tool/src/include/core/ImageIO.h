#ifndef __IMAGE_IO_H__
#define __IMAGE_IO_H__

#include <Magick++.h>

#include "core/Convert.h"

class ImageIO {
  public:
    static Cairo::RefPtr<Cairo::ImageSurface> read(const std::string& file) {
      Magick::Image magickImage;
      magickImage.read(file);

      Cairo::RefPtr<Cairo::ImageSurface> image =
        Convert::magick_to_cairo(&magickImage);

      return image;
    }

  private:
    ImageIO();
};

#endif
