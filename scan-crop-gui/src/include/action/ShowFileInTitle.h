#ifndef __SHOW_FILE_IN_TITLE_H__
#define __SHOW_FILE_IN_TITLE_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

class ShowFileInTitle : public Action {

  public:
    ShowFileInTitle(const std::string& path) : 
      path(path) {

        // get builder object
        Glib::RefPtr<Gtk::Builder> builder =
          GuiDescription::get_instance()->get_builder();

        // get dimensions of drawing area
        builder->get_widget("mainWindow", this->window);


      }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      this->window->set_title(path);

    }

    /*----------------------------------------------------------------------*/

  private:

    std::string path;

    Gtk::ApplicationWindow *window;

    /*----------------------------------------------------------------------*/
};
#endif
