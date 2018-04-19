#ifndef __HIDE_SEGMENTATION_FRAME_H__
#define __HIDE_SEGMENTATION_FRAME_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

class HideSegmentationFrame : public Action {

  public:
    HideSegmentationFrame() {

        // get builder object
        Glib::RefPtr<Gtk::Builder> builder =
          GuiDescription::get_instance()->get_builder();

        builder->get_widget("segmentationFrame", this->segmentationFrame);

      }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      this->segmentationFrame->hide();

    }

    /*----------------------------------------------------------------------*/

  private:

    Gtk::Frame *segmentationFrame;

    /*----------------------------------------------------------------------*/
};
#endif
