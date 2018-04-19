#ifndef __SHOW_FILTER_WINDOW_H__
#define __SHOW_FILTER_WINDOW_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

class ShowFilterWindow: public Action {

  public:

    ShowFilterWindow() {

      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get the dialog ui element
      builder->get_widget("filterWindow", this->filterWindow);
    }

    /*-----------------------------------------------------------------------*/

    virtual void execute() {
      filterWindow->show();
    }

    /*-----------------------------------------------------------------------*/
  private:
    /** ui element for the dialog */
    Gtk::Window* filterWindow;
};

#endif
