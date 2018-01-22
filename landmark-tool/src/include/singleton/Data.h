#ifndef __DATA_H__
#define __DATA_H__

#include <memory>
#include <unordered_map>
#include <vector>

#include "image/Image.h"

#include "singleton/GuiDescription.h"
#include "core/ImageStack.h"
#include "core/Point.h"

class Data {
  public:

    /*----------------------------------------------------------------------*/

    static std::shared_ptr<Data> get_instance() {
      if(instance == nullptr) {
        instance = std::shared_ptr<Data>(new Data);
      }
      return instance;
    }

    /*----------------------------------------------------------------------*/

    Cairo::RefPtr<Cairo::ImageSurface> get_slice(int index) const {
      auto slices = this->imageRepresentations.at(this->viewMode);
      return slices.at(index);
    }

    /*----------------------------------------------------------------------*/

    Cairo::RefPtr<Cairo::ImageSurface> get_segmentation_slice(int index) const {

      auto slices = this->segmentationRepresentations.at(this->viewMode);
      return slices.at(index);

    }

    /*----------------------------------------------------------------------*/

    bool stack_present() const {
      return this->stack != nullptr;
    }

    /*----------------------------------------------------------------------*/

    bool segmentation_stack_present() const {

      return this->segmentation_stack != nullptr;

    }

    /*----------------------------------------------------------------------*/

    void set_view_mode(int mode) {
      this->viewMode = mode;
    }

    /*----------------------------------------------------------------------*/

    const Point& get_dimensions() const {
      return this->dimensions;
    }

    /*----------------------------------------------------------------------*/

    void set_image_stack(const std::shared_ptr<ImageStack>& stack) {

      this->stack = stack;

      this->dimensions.set_canonical_x(stack->get_width());
      this->dimensions.set_canonical_y(stack->get_height());
      this->dimensions.set_canonical_z(stack->get_depth());

      this->imageRepresentations[0] = stack->get_xy_slices();
      this->imageRepresentations[1] = stack->get_zx_slices();
      this->imageRepresentations[2] = stack->get_zy_slices();

      this->segmentation_stack = nullptr;

      // emit signal that the image stack was set
      this->image_stack_set.emit();
    }

    /*----------------------------------------------------------------------*/

    void set_segmentation_stack(const std::shared_ptr<ImageStack>& stack) {

      this->segmentation_stack = stack;

      this->segmentationRepresentations[0] = stack->get_xy_slices();
      this->segmentationRepresentations[1] = stack->get_zx_slices();
      this->segmentationRepresentations[2] = stack->get_zy_slices();

      // emit signal that the segmentation stack was set
      this->segmentation_stack_set.emit();
    }

    /*----------------------------------------------------------------------*/


    void set_original_image(const Image& image) {

      this->originalImage = image;

    }

    /*----------------------------------------------------------------------*/

    const Image& get_original_image() const {

      return this->originalImage;

    }

    /*----------------------------------------------------------------------*/

    void set_current_image(const Image& image) {

      this->currentImage = image;

    }

    /*----------------------------------------------------------------------*/

    const Image& get_current_image() const {

      return this->currentImage;

    }

    /*----------------------------------------------------------------------*/
    /** signal that is emitted if the image stack was set */
    sigc::signal<void> image_stack_set;

    sigc::signal<void> segmentation_stack_set;

    /*----------------------------------------------------------------------*/


  private:
    Data() {
      viewMode = 0;
    }

    /*----------------------------------------------------------------------*/

    std::unordered_map<int, std::vector<Cairo::RefPtr<Cairo::ImageSurface> > >
      imageRepresentations;

    std::unordered_map<int, std::vector<Cairo::RefPtr<Cairo::ImageSurface> > >
      segmentationRepresentations;

    std::shared_ptr<ImageStack> stack;
    std::shared_ptr<ImageStack> segmentation_stack;

    Point dimensions;

    static std::shared_ptr<Data> instance;
    int viewMode;

    Image currentImage;

    Image originalImage;

};
#endif
