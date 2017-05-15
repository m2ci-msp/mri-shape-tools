#ifndef __INTERNAL_PLOT_H__
#define __INTERNAL_PLOT_H__

#include <memory>

class InternalPlot {
  public:

    /*----------------------------------------------------------------------*/

    static std::shared_ptr<InternalPlot> get_instance() {
      if(instance == nullptr) {
        instance = std::shared_ptr<InternalPlot> (new InternalPlot);
      }
      return instance;
    }

    /*----------------------------------------------------------------------*/

    void set_plot( Cairo::RefPtr<Cairo::ImageSurface> image) {
      this->plot = image;
      this->internal_plot_changed.emit();

    }

    /*----------------------------------------------------------------------*/

    Cairo::RefPtr<Cairo::ImageSurface> get_plot() const {
      return this->plot;
    }

    /*----------------------------------------------------------------------*/

    int get_width() const {
      return this->plot->get_width();
    }

    int get_height() const {
      return this->plot->get_height();
    }


    /*----------------------------------------------------------------------*/

    /** signal that is emitted if the internal plot changes */
    sigc::signal<void> internal_plot_changed;

    /*----------------------------------------------------------------------*/

  private:

    /*----------------------------------------------------------------------*/

    Cairo::RefPtr<Cairo::ImageSurface> plot;

    static std::shared_ptr<InternalPlot> instance;

};
#endif
