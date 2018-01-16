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

        builder->get_widget("addLandmarksButton", this->addLandmarks);
        builder->get_widget("saveLandmarksButton", this->saveLandmarks);
        builder->get_widget("addSegmentationButton", this->addSegmentation);


      }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      this->addLandmarks->show();
      this->saveLandmarks->show();
      this->addSegmentation->show();

    }

    /*----------------------------------------------------------------------*/

  private:

    Gtk::ToolButton *addLandmarks;
    Gtk::ToolButton *saveLandmarks;
    Gtk::ToolButton *addSegmentation;

    /*----------------------------------------------------------------------*/
};
#endif
