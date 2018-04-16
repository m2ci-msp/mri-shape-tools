#ifndef __SHOW_SEGMENTATION_WINDOW_H__
#define __SHOW_SEGMENTATION_WINDOW_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

class ShowSegmentationWindow: public Action {

  public:

    ShowSegmentationWindow() {

      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get the dialog ui element
      builder->get_widget("segmentationWindow", this->segmentationWindow);
    }

    /*-----------------------------------------------------------------------*/

    virtual void execute() {
      segmentationWindow->show();
    }

    /*-----------------------------------------------------------------------*/
  private:

    Gtk::Window* segmentationWindow;

};

#endif
