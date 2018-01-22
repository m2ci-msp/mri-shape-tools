#ifndef __APPLY_FILTERS_BUTTON_OBSERVER_H__
#define __APPLY_FILTERS_BUTTON_OBSERVER_H__

#include <gtkmm/button.h>

#include "singleton/GuiDescription.h"
#include "action/ApplyFilters.h"
#include "action/ShowErrorMessage.h"

class ApplyFiltersButtonObserver {

 public:
  ApplyFiltersButtonObserver() { 

    // query GUI description for builder
    Glib::RefPtr<Gtk::Builder> builder =
      GuiDescription::get_instance()->get_builder();

    // get the ui element
    builder->get_widget("applyFilterButton", this->button);

    // connect handler
    this->clickedHandler = this->button->signal_clicked().connect(
                                                                  sigc::mem_fun(*this,
                                                                                &ApplyFiltersButtonObserver::clicked));

  }

  /*----------------------------------------------------------------------*/

  ~ApplyFiltersButtonObserver() {

    this->clickedHandler.disconnect();

  }

  /*----------------------------------------------------------------------*/

  void clicked() {

    try{

      ApplyFilters applyFilters;
      applyFilters.execute();

    }
    catch (const std::exception& e) {

      ShowErrorMessage message("Could not apply filters.\n", e);
      message.execute();

    }
  }

  /*----------------------------------------------------------------------*/

 private:

  Gtk::Button* button;
  sigc::connection clickedHandler;

};

#endif
