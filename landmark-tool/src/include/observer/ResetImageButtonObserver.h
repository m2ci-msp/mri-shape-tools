#ifndef __RESET_IMAGE_BUTTON_OBSERVER_H__
#define __RESET_IMAGE_BUTTON_OBSERVER_H__

#include <gtkmm/button.h>

#include "singleton/GuiDescription.h"
#include "action/ResetImage.h"

class ResetImageButtonObserver {

 public:
  ResetImageButtonObserver() { 

    // query GUI description for builder
    Glib::RefPtr<Gtk::Builder> builder =
      GuiDescription::get_instance()->get_builder();

    // get the ui element
    builder->get_widget("resetImageButton", this->button);

    // connect handler
    this->clickedHandler = this->button->signal_clicked().connect(
                                                                  sigc::mem_fun(*this,
                                                                                &ResetImageButtonObserver::clicked));

  }

  /*----------------------------------------------------------------------*/

  ~ResetImageButtonObserver() {

    this->clickedHandler.disconnect();

  }

  /*----------------------------------------------------------------------*/

  void clicked() {

    ResetImage reset;
    reset.execute();

  }

  /*----------------------------------------------------------------------*/

 private:

  Gtk::Button* button;
  sigc::connection clickedHandler;

};

#endif
