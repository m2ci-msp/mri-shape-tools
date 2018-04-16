#ifndef __SAVE_SLICE_H__
#define __SAVE_SLICE_H__

#include "action/Action.h"
#include "singleton/InternalPlot.h"

class SaveSlice : public Action {

  public:
    SaveSlice(const std::string& path) :
      path(path) {
      }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      Cairo::RefPtr<Cairo::ImageSurface> image = InternalPlot::get_instance()->get_plot();
      image->write_to_png(this->path);
    }

    /*----------------------------------------------------------------------*/

  private:
    std::string path;
};
#endif
