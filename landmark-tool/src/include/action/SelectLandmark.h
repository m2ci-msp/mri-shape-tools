#ifndef __SELECT_LANDMARK_H__
#define __SELECT_LANDMARK_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/LandmarkPool.h"

/** Selects the landmark that is active in the tree view
 * in the landmark pool */
class SelectLandmark : public Action {

  public:
    /*----------------------------------------------------------------------*/
    SelectLandmark() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      Gtk::TreeView* view;
      builder->get_widget("landmarkTreeView", view);

      Glib::RefPtr<Gtk::TreeSelection> selection = view->get_selection();
      Gtk::TreeModel::iterator selectedRow = selection->get_selected();

      if(!selectedRow) {
        this->id = -1;
      }
      else {
        selectedRow->get_value(1, this->id);
      }
    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {
      LandmarkPool::get_instance()->select_id(this->id);
    }

    /*----------------------------------------------------------------------*/

  private:
    int id;
};
#endif
