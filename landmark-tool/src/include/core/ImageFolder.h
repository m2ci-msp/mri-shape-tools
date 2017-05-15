#ifndef __IMAGE_FOLDER_H__
#define __IMAGE_FOLDER_H__

#include <regex>
#include <exception>
#include <vector>
#include <list>

#include "ImageIO.h"

/** Class representing an image folder on the filesystem */
class ImageFolder {
  public:
    ImageFolder(const std::string& path) : path(path) {
    }

    /*----------------------------------------------------------------------*/

    std::shared_ptr<ImageStack> read_image_stack() {
      // query path for contained image files
      auto imageNames = get_sorted_image_list();

      if(imageNames.empty() ) {
        throw std::logic_error("No images found in folder.");
      }

      auto xySlices = read_images(imageNames);

      std::shared_ptr<ImageStack> stack =
        std::make_shared<ImageStack>(xySlices);

      return stack;
    }

    /*----------------------------------------------------------------------*/

  private:
    ImageFolder();

    /*----------------------------------------------------------------------*/

    /* extract the list of images contained in the folder */
    std::list<Glib::ustring> get_sorted_image_list() {

      // read PGM images from the selected folder
      Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(this->path);
      Glib::RefPtr<Gio::FileEnumerator> childEnumeration = 
        file->enumerate_children(G_FILE_ATTRIBUTE_STANDARD_NAME);

      std::list<Glib::ustring> images;
      Glib::RefPtr<Gio::FileInfo> fileInfo;

      std::regex reg(".*\\.pgm");

      while ((fileInfo = childEnumeration->next_file()) )
      {
        if( std::regex_match(fileInfo->get_name(), reg) ) {
          images.push_back(fileInfo->get_name());
        }
      }

      images.sort();

      return images;
    }

    /*----------------------------------------------------------------------*/

    std::vector<Cairo::RefPtr<Cairo::ImageSurface> > read_images(
        const std::list<Glib::ustring> &imageNames
        ) {

      std::vector<Cairo::RefPtr<Cairo::ImageSurface> > images;

      for( auto& file: imageNames) {
        // read the pgm file
        const auto image = ImageIO::read(path + "/" + file);
        images.push_back(image);
      }

      return images;

    }

    /*----------------------------------------------------------------------*/

    std::string path;

};

#endif
