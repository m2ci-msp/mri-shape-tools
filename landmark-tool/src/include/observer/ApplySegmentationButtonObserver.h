#ifndef __APPLY_SEGMENTATION_BUTTON_OBSERVER_H__
#define __APPLY_SEGMENTATION_BUTTON_OBSERVER_H__

#include <gtkmm/button.h>

#include "singleton/GuiDescription.h"
#include "action/ApplySegmentation.h"
#include "action/ShowErrorMessage.h"
#include "action/ShowSegmentationFrame.h"

class ApplySegmentationButtonObserver {

 public:
  ApplySegmentationButtonObserver() { 

    // query GUI description for builder
    Glib::RefPtr<Gtk::Builder> builder =
      GuiDescription::get_instance()->get_builder();

    // get the ui element
    builder->get_widget("applySegmentationButton", this->button);

    // connect handler
    this->clickedHandler = this->button->signal_clicked().connect(
                                                                  sigc::mem_fun(*this,
                                                                                &ApplySegmentationButtonObserver::clicked));

  }

  /*----------------------------------------------------------------------*/

  ~ApplySegmentationButtonObserver() {

    this->clickedHandler.disconnect();

  }

  /*----------------------------------------------------------------------*/

  void clicked() {

    try{

      ApplySegmentation applySegmentation;
      applySegmentation.execute();

      ShowSegmentationFrame show;
      show.execute();

    }
    catch (const std::exception& e) {

      ShowErrorMessage message("Could not apply segmentation.\n", e);
      message.execute();

    }
  }

  /*----------------------------------------------------------------------*/

 private:

  Gtk::Button* button;
  sigc::connection clickedHandler;

};

#endif
