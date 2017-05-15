#ifndef __SELECT_TREEVIEW_ENTRY_H__
#define __SELECT_TREEVIEW_ENTRY_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/LandmarkPool.h"

/** Select the landmark that is active in the landmark pool in the
 * treeview
 */
class SelectTreeViewEntry : public Action {

  public:
    /*----------------------------------------------------------------------*/
    SelectTreeViewEntry() { 
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      builder->get_widget("landmarkTreeView", this->view);

      this->listStore = 
        Glib::RefPtr<Gtk::ListStore>::cast_dynamic(
            builder->get_object("landmarkListStore"));

      this->column = 
        Glib::RefPtr<Gtk::TreeViewColumn>::cast_dynamic(
            builder->get_object("landmarkNameColumn"));


      auto mark = LandmarkPool::get_instance()->get_selected_landmark();
      this->id = ( !mark)? -1 : mark->get_id();

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      // do not update the tree view if the entry is already selected
      // otherwise we might end up in an infinite recursion
      Glib::RefPtr<Gtk::TreeSelection> selection = view->get_selection();
      Gtk::TreeModel::iterator selectedRow = selection->get_selected();

      if( selectedRow ) {
        int selectedId = 0;
        selectedRow->get_value(1, selectedId);

        if( selectedId == this->id) {
          return;
        }
      }

      // find the row belonging to the current landmark and select it
      auto rows = this->listStore->children();

      if( !rows ) {
        return;
      }

      for(const auto& row: rows ) {
        int mark_id = 0;
        row->get_value(1, mark_id);

        if( mark_id == this->id ) {
          Gtk::TreePath path(row);
          view->set_cursor(path, *this->column.operator->(), false);

          return;
        }

      }

      return;

    }

    /*----------------------------------------------------------------------*/

  private:

    Gtk::TreeView* view;
    Glib::RefPtr<Gtk::ListStore> listStore;
    Glib::RefPtr<Gtk::TreeViewColumn> column;
    int id;
};
#endif
