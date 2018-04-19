#ifndef __SAVE_FILTER_H__
#define __SAVE_FILTER_H__

#include <json/json.h>

#include "action/FilterAction.h"
#include "singleton/GuiDescription.h"

/* Saves filter description to file */
class SaveFilter : public FilterAction {

public:
  SaveFilter(const std::string& path) : path(path) {

    // get builder object
    Glib::RefPtr<Gtk::Builder> builder =
      GuiDescription::get_instance()->get_builder();

    this->filterText =
      Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(
                                                  builder->get_object("filterTextBuffer"));

  }

  /*----------------------------------------------------------------------*/

  virtual void execute() {

    Json::Value jsonFilter = add_landmarks_if_needed(filterText->get_text());

    std::ofstream outFile(this->path);

    outFile << jsonFilter << std::endl;

    outFile.close();

  }

  /*----------------------------------------------------------------------*/

private:

  Glib::RefPtr<Gtk::TextBuffer> filterText;

  std::string path;

};
#endif
