#ifndef __SAVE_SEGMENTATION_H__
#define __SAVE_SEGMENTATION_H__

#include <json/json.h>

#include "action/FilterAction.h"
#include "singleton/GuiDescription.h"

/** Saves landmarks to file */
class SaveSegmentation : public FilterAction {

public:
  SaveSegmentation(const std::string& path) : path(path) {

    // get builder object
    Glib::RefPtr<Gtk::Builder> builder =
      GuiDescription::get_instance()->get_builder();

    this->segmentationText =
      Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(
                                                  builder->get_object("segmentationTextBuffer"));

  }

  /*----------------------------------------------------------------------*/

  virtual void execute() {

    Json::Value jsonSegmentation = add_landmarks_if_needed(segmentationText->get_text());

    std::ofstream outFile(this->path);

    outFile << jsonSegmentation << std::endl;

    outFile.close();

  }

  /*----------------------------------------------------------------------*/

private:

  Glib::RefPtr<Gtk::TextBuffer> segmentationText;

  std::string path;

};
#endif
