#ifndef __DELETE_LANDMARK_H__
#define __DELETE_LANDMARK_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/LandmarkPool.h"
#include "singleton/EventPosition.h"
#include "core/Point.h"

/** Deletes the currently selected landmark */
class DeleteLandmark : public Action {

  public:
    /*----------------------------------------------------------------------*/
    DeleteLandmark() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      this->listStore = 
        Glib::RefPtr<Gtk::ListStore>::cast_dynamic(
            builder->get_object("landmarkListStore"));

      builder->get_widget("landmarkTreeView", this->view);

      this->mark = LandmarkPool::get_instance()->get_selected_landmark(); 
    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      if(!mark) {
        return;
      }

      delete_landmark_in_liststore();
      delete_landmark_in_pool();

    }

    /*----------------------------------------------------------------------*/

  private:

    void delete_landmark_in_pool() {
      LandmarkPool::get_instance()->delete_landmark(mark);
    }

    /*----------------------------------------------------------------------*/

    void delete_landmark_in_liststore() {
      // find the row belonging to the current landmark and delete it
      auto rows = this->listStore->children();
      int mark_id = this->mark->get_id();

      for(const auto& row: rows ) {
        int current_id = 0;
        row->get_value(1, current_id);

        if( current_id == mark_id ) {
          this->listStore->erase(row);
          return;
        }

      }

      return;

    }
    /*----------------------------------------------------------------------*/

    Landmark* mark;
    Glib::RefPtr<Gtk::ListStore> listStore;
    Gtk::TreeView* view;
};
#endif
