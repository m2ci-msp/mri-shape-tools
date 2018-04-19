#ifndef __SAVE_CROP_SPECIFICATION_H__
#define __SAVE_CROP_SPECIFICATION_H__

#include <iostream>

#include "singleton/GuiDescription.h"
#include "action/Action.h"
#include "singleton/Data.h"


class SaveCropSpecification : public Action {

public:

  /*----------------------------------------------------------------------*/

  SaveCropSpecification(const std::string& path) : path(path) {
    // get builder object
    Glib::RefPtr<Gtk::Builder> builder =
      GuiDescription::get_instance()->get_builder();

    // get values for cropping
    this->min = { {
	(Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
						     builder->get_object("minXAdjustment")))->get_value(),
	(Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
						     builder->get_object("minYAdjustment")))->get_value(),
	(Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
						     builder->get_object("minZAdjustment")))->get_value()
      }};

    this->max = { {
	(Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
						     builder->get_object("maxXAdjustment")))->get_value(),
	(Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
						     builder->get_object("maxYAdjustment")))->get_value(),
	(Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
						     builder->get_object("maxZAdjustment")))->get_value()
      }};

  }

  /*----------------------------------------------------------------------*/

  virtual void execute() {

    std::ofstream outFile(this->path, std::ios_base::out | std::ios_base::trunc);

    outFile << "speaker{" << std::endl;
    outFile << "  cropToVocalTract{" << std::endl;
    outFile << "    minX=" << this->min.at(0) << std::endl;
    outFile << "    minY=" << this->min.at(1) << std::endl;
    outFile << "    minZ=" << this->min.at(2) << std::endl;
    outFile << "    maxX=" << this->max.at(0) << std::endl;
    outFile << "    maxY=" << this->max.at(1) << std::endl;
    outFile << "    maxZ=" << this->max.at(2) << std::endl;
    outFile << "  }" << std::endl;
    outFile << "}" << std::endl;

    outFile.close();

  }

  /*----------------------------------------------------------------------*/


private:

  /*----------------------------------------------------------------------*/

  std::array<double, 3> min;
  std::array<double, 3> max;

  std::string path;

  /*----------------------------------------------------------------------*/

};

#endif
