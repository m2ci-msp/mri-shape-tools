#ifndef __SHOW_SEGMENTATION_FRAME_H__
#define __SHOW_SEGMENTATION_FRAME_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

class ShowSegmentationFrame : public Action {

  public:
    ShowSegmentationFrame() {

        // get builder object
        Glib::RefPtr<Gtk::Builder> builder =
          GuiDescription::get_instance()->get_builder();

        builder->get_widget("segmentationFrame", this->segmentationFrame);

      }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      this->segmentationFrame->show();

    }

    /*----------------------------------------------------------------------*/

  private:

    Gtk::Frame *segmentationFrame;

    /*----------------------------------------------------------------------*/
};
#endif
