#ifndef __SHOW_MORE_ACTIONS_H__
#define __SHOW_MORE_ACTIONS_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

class ShowMoreActions : public Action {

  public:
    ShowMoreActions() {

        // get builder object
        Glib::RefPtr<Gtk::Builder> builder =
          GuiDescription::get_instance()->get_builder();

        builder->get_widget("saveCropSpecificationButton", this->saveButton);

      }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      this->saveButton->set_sensitive();

    }

    /*----------------------------------------------------------------------*/

  private:

    Gtk::ToolButton *saveButton;

};
#endif
