#ifndef __REFRESH_LANDMARKS_VIEW_H__
#define __REFRESH_LANDMARKS_VIEW_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/LandmarkPool.h"

/** Refreshes the treeview displaying the landmarks */
class RefreshLandmarksView : public Action {

  public:
    /*----------------------------------------------------------------------*/
    RefreshLandmarksView() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      this->listStore = 
        Glib::RefPtr<Gtk::ListStore>::cast_dynamic(
            builder->get_object("landmarkListStore"));

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      auto landmarks = LandmarkPool::get_instance()->get_all_landmarks();
      this->listStore->clear();

      for( const auto& mark : landmarks ) {
        Gtk::TreeModel::iterator row = this->listStore->append();
        row->set_value(0, (Glib::ustring) mark->get_name());
        row->set_value(1, mark->get_id());
      }

    }

    /*----------------------------------------------------------------------*/

  private:
    Glib::RefPtr<Gtk::ListStore> listStore;
};
#endif
