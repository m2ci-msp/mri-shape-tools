#ifndef __APPLY_SEGMENTATION_H__
#define __APPLY_SEGMENTATION_H__

#include <json/json.h>

#include "action/Action.h"
#include "core/SegmentationStackBuilder.h"
#include "singleton/Data.h"
#include "singleton/GuiDescription.h"
#include "image-modify/ApplyModifications.h"

class ApplySegmentation : public Action {

public:
  /*----------------------------------------------------------------------*/

  ApplySegmentation() {

    // get builder object
    Glib::RefPtr<Gtk::Builder> builder =
      GuiDescription::get_instance()->get_builder();

    this->segmentationText = 
      Glib::RefPtr<Gtk::TextBuffer>::cast_dynamic(
                                                  builder->get_object("segmentationTextBuffer"));

  }

  /*----------------------------------------------------------------------*/

  virtual void execute() {

    Image image = Data::get_instance()->get_current_image();

    std::string modifiedText = add_landmarks_if_needed(segmentationText->get_text());

    ApplyModifications(image).apply(modifiedText);

    SegmentationStackBuilder builder;
    Data::get_instance()->set_segmentation_stack(builder.build_from(image));

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

  Glib::RefPtr<Gtk::TextBuffer> segmentationText;

};
#endif
