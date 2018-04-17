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

        builder->get_widget("addLandmarksButton", this->addLandmarksButton);
        builder->get_widget("saveLandmarksButton", this->saveLandmarksButton);

        builder->get_widget("addLandmarksItem", this->addLandmarksItem);
        builder->get_widget("saveLandmarksItem", this->saveLandmarksItem);
        builder->get_widget("loadSegmentationItem", this->loadSegmentationItem);
        builder->get_widget("saveSliceItem", this->saveSliceItem);
        builder->get_widget("showFilterItem", this->showFilterItem);
        builder->get_widget("showSegmentationItem", this->showSegmentationItem);


      }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      this->addLandmarksButton->set_sensitive();
      this->saveLandmarksButton->set_sensitive();

      this->addLandmarksItem->set_sensitive();
      this->saveLandmarksItem->set_sensitive();
      this->loadSegmentationItem->set_sensitive();
      this->saveSliceItem->set_sensitive();
      this->showFilterItem->set_sensitive();
      this->showSegmentationItem->set_sensitive();

    }

    /*----------------------------------------------------------------------*/

  private:

    Gtk::ToolButton *addLandmarksButton;
    Gtk::ToolButton *saveLandmarksButton;

    Gtk::MenuItem *addLandmarksItem;
    Gtk::MenuItem *saveLandmarksItem;
    Gtk::MenuItem *loadSegmentationItem;
    Gtk::MenuItem *saveSliceItem;
    Gtk::MenuItem *showFilterItem;
    Gtk::MenuItem *showSegmentationItem;

    /*----------------------------------------------------------------------*/
};
#endif
