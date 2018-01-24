#ifndef __APPLY_FILTERS_H__
#define __APPLY_FILTERS_H__

#include <json/json.h>

#include "action/Action.h"
#include "core/ImageStackBuilder.h"
#include "singleton/Data.h"
#include "singleton/GuiDescription.h"
#include "image-modify/ApplyModifications.h"

class ApplyFilters : public Action {

public:
  /*----------------------------------------------------------------------*/

  ApplyFilters() {

    // get builder object
    Glib::RefPtr<Gtk::Builder> builder =
      GuiDescription::get_instance()->get_builder();

    this->filtersText = 
      Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(
                                                  builder->get_object("filterTextBuffer"));

  }

  /*----------------------------------------------------------------------*/

  virtual void execute() {

    Image image = Data::get_instance()->get_current_image();

    std::string modifiedText = add_landmarks_if_needed(filtersText->get_text());

    ApplyModifications(image).apply(modifiedText);

    Data::get_instance()->set_current_image(image);

    ImageStackBuilder builder;
    Data::get_instance()->set_image_stack(builder.build_from(image));

  }

  /*----------------------------------------------------------------------*/

private:

  std::string add_landmarks_if_needed(const std::string& originalString) const {

    Json::Value description;
    Json::Reader reader;

    reader.parse(originalString, description);

    for(Json::Value& entry: description) {

      const std::string action = entry["action"].asString();
      Json::Value& options = entry["options"];

      if( action == "segment" && options["type"].asString() == "with landmarks") {

        Json::Value landmarks(Json::arrayValue);

        for(const auto& mark: LandmarkPool::get_instance()->get_all_landmarks() ) {

          Json::Value landmark(Json::objectValue);

          landmark["x"] = Json::Value(mark->get_position().get_canonical_x());
          landmark["y"] = Json::Value(mark->get_position().get_canonical_y());
          landmark["z"] = Json::Value(mark->get_position().get_canonical_z());

          landmarks.append(landmark);

        }

        options["landmarks"] = landmarks;

      }


    }

    return Json::FastWriter().write(description);

  }

  Glib::RefPtr<Gtk::TextBuffer> filtersText;

};
#endif
