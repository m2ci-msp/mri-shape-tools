#include <gtkmm.h>
#include "singleton/GuiDescription.h"
#include "static_initializations.h"
#include "observer/Observatory.h"

int main(int, char*[])
{
  //  Gtk::Main kit(argc, argv);
  Glib::RefPtr<Gtk::Application> app =
    Gtk::Application::create(Glib::ustring(), Gio::APPLICATION_NON_UNIQUE);

  // get the builder object
  Glib::RefPtr<Gtk::Builder> builder =
    GuiDescription::get_instance()->get_builder();

  // get main window
  Gtk::Window* window;
  builder->get_widget("mainWindow", window);

  // register observers
  Observatory<OpenScanButtonObserver>::create();
  Observatory<AddSegmentationButtonObserver>::create();
  Observatory<OpenScanDialogObserver>::create();
  Observatory<AddSegmentationDialogObserver>::create();
  Observatory<AlphaObserver>::create();
  Observatory<SegmentationObserver>::create();
  Observatory<ImageStackObserver>::create();
  Observatory<DrawingAreaObserver>::create();
  Observatory<ScrollbarObserver>::create();
  Observatory<ViewModeObserver>::create();
  Observatory<SliceObserver>::create();
  Observatory<InternalPlotObserver>::create();
  Observatory<ZoomObserver>::create();
  Observatory<LandmarksObserver>::create();
  Observatory<LandmarkTreeViewObserver>::create();
  Observatory<SaveLandmarksButtonObserver>::create();
  Observatory<SaveLandmarksDialogObserver>::create();
  Observatory<AddLandmarksButtonObserver>::create();
  Observatory<AddLandmarksDialogObserver>::create();
  Observatory<ErrorMessageDialogObserver>::create();
  Observatory<MainWindowObserver>::create();
  Observatory<ApplyFiltersButtonObserver>::create();

  // show main window
  return app->run(*window);

}
