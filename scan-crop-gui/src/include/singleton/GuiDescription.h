#ifndef __GUI_DESCRIPTION_H__
#define __GUI_DESCRIPTION_H__

#include <memory>
#include <iostream>

class GuiDescription {
  public:

    /*----------------------------------------------------------------------*/

    static std::shared_ptr<GuiDescription> get_instance() {
      if(instance == nullptr) {
        instance = std::shared_ptr<GuiDescription> (new GuiDescription);
      }
      return instance;
    }

    /*----------------------------------------------------------------------*/

    Glib::RefPtr<Gtk::Builder> get_builder() {
      return this->builder;
    }

    /*----------------------------------------------------------------------*/

  private:

    GuiDescription() {
      builder = Gtk::Builder::create();
      try
      {
        // open ui file
        builder->add_from_file(UIDIR +
            std::string("/ui_description_scan_crop.glade"));
      }
      catch(const Glib::FileError& ex)
      {
        std::cerr << "FileError: " << ex.what() << std::endl;
        exit(1);
      }
      catch(const Gtk::BuilderError& ex)
      {
        std::cerr << "BuilderError: " <<
          ex.what() << std::endl;
        exit(1);
      }

    }

    /*----------------------------------------------------------------------*/

    Glib::RefPtr<Gtk::Builder> builder;
    static std::shared_ptr<GuiDescription> instance;

};
#endif
