#ifndef __ADD_LANDMARK_H__
#define __ADD_LANDMARK_H__

#include <armadillo>

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/LandmarkPool.h"
#include "singleton/EventPosition.h"
#include "core/Point.h"

/** Adds a landmark to the landmark pool and the liststore */
class AddLandmark : public Action {

  public:
    /*----------------------------------------------------------------------*/
    AddLandmark() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      this->listStore = 
        Glib::RefPtr<Gtk::ListStore>::cast_dynamic(
            builder->get_object("landmarkListStore"));

      this->column = 
        Glib::RefPtr<Gtk::TreeViewColumn>::cast_dynamic(
            builder->get_object("landmarkNameColumn"));

      builder->get_widget("landmarkTreeView", this->view);

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      if( !EventPosition::get_instance()->is_valid() ) {
        // do nothing if the position is not valid
        return;
      }

      auto point = EventPosition::get_instance()->get(); 

      // convert point to the center of the pixel containing it
      point.set_x( (int) point.get_x() + 0.5);
      point.set_y( (int) point.get_y() + 0.5);
      point.set_z( (int) point.get_z() + 0.5);

      // create landmark and add it to the landmark pool
      auto mark = LandmarkPool::get_instance()->create_landmark(point); 

      // add landmark to liststore
      Gtk::TreeModel::iterator row = this->listStore->append();
      row->set_value(0, (Glib::ustring) mark->get_name());
      row->set_value(1, mark->get_id());

      // set cursor in tree view and activate editing
      Gtk::TreePath path(row);
      view->set_cursor(path, *this->column.operator->(), true);

    }

    /*----------------------------------------------------------------------*/

  private:
    Glib::RefPtr<Gtk::ListStore> listStore;
    Gtk::TreeView* view;
    Glib::RefPtr<Gtk::TreeViewColumn> column;
};
#endif
