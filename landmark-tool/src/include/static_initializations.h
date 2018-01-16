#ifndef __STATIC_INITIALIZATIONS_H__
#define __STATIC_INITIALIZATIONS_H__

#include <memory>
#include "singleton/GuiDescription.h"
#include "singleton/InternalPlot.h"
#include "singleton/EventPosition.h"
#include "singleton/LandmarkPool.h"
#include "singleton/CoordinateSystem.h"
#include "singleton/EditMode.h"
#include "observer/Observatory.h"
#include "observer/OpenScanButtonObserver.h"
#include "observer/OpenScanDialogObserver.h"
#include "observer/ImageStackObserver.h"
#include "observer/DrawingAreaObserver.h"
#include "observer/ScrollbarObserver.h"
#include "observer/ViewModeObserver.h"
#include "observer/SliceObserver.h"
#include "observer/InternalPlotObserver.h"
#include "observer/ZoomObserver.h"
#include "observer/LandmarksObserver.h"
#include "observer/LandmarkTreeViewObserver.h"
#include "observer/SaveLandmarksButtonObserver.h"
#include "observer/SaveLandmarksDialogObserver.h"
#include "observer/AddLandmarksButtonObserver.h"
#include "observer/AddLandmarksDialogObserver.h"
#include "observer/ErrorMessageDialogObserver.h"
#include "observer/MainWindowObserver.h"
#include "observer/AddSegmentationDialogObserver.h"
#include "observer/AddSegmentationButtonObserver.h"
#include "observer/AlphaObserver.h"
#include "observer/SegmentationObserver.h"
#include "core/Landmark.h"
#include "core/IdPool.h"

template<> std::shared_ptr<OpenScanButtonObserver>
Observatory<OpenScanButtonObserver>::observer = nullptr;

template<> std::shared_ptr<AddSegmentationButtonObserver>
Observatory<AddSegmentationButtonObserver>::observer = nullptr;

template<> std::shared_ptr<OpenScanDialogObserver>
Observatory<OpenScanDialogObserver>::observer = nullptr;

template<> std::shared_ptr<AddSegmentationDialogObserver>
Observatory<AddSegmentationDialogObserver>::observer = nullptr;

template<> std::shared_ptr<AlphaObserver>
Observatory<AlphaObserver>::observer = nullptr;

template<> std::shared_ptr<SegmentationObserver>
Observatory<SegmentationObserver>::observer = nullptr;

template<> std::shared_ptr<ImageStackObserver>
Observatory<ImageStackObserver>::observer = nullptr;

template<> std::shared_ptr<DrawingAreaObserver>
Observatory<DrawingAreaObserver>::observer = nullptr;

template<> std::shared_ptr<ScrollbarObserver>
Observatory<ScrollbarObserver>::observer = nullptr;

template<> std::shared_ptr<ViewModeObserver>
Observatory<ViewModeObserver>::observer = nullptr;

template<> std::shared_ptr<SliceObserver>
Observatory<SliceObserver>::observer = nullptr;

template<> std::shared_ptr<InternalPlotObserver>
Observatory<InternalPlotObserver>::observer = nullptr;

template<> std::shared_ptr<ZoomObserver>
Observatory<ZoomObserver>::observer = nullptr;

template<> std::shared_ptr<LandmarksObserver>
Observatory<LandmarksObserver>::observer = nullptr;

template<> std::shared_ptr<LandmarkTreeViewObserver>
Observatory<LandmarkTreeViewObserver>::observer = nullptr;

template<> std::shared_ptr<SaveLandmarksButtonObserver>
Observatory<SaveLandmarksButtonObserver>::observer = nullptr;

template<> std::shared_ptr<SaveLandmarksDialogObserver>
Observatory<SaveLandmarksDialogObserver>::observer = nullptr;

template<> std::shared_ptr<AddLandmarksButtonObserver>
Observatory<AddLandmarksButtonObserver>::observer = nullptr;

template<> std::shared_ptr<AddLandmarksDialogObserver>
Observatory<AddLandmarksDialogObserver>::observer = nullptr;

template<> std::shared_ptr<ErrorMessageDialogObserver>
Observatory<ErrorMessageDialogObserver>::observer = nullptr;

template<> std::shared_ptr<MainWindowObserver>
Observatory<MainWindowObserver>::observer = nullptr;

template<> IdPool<Landmark>* IdPool<Landmark>::instance = nullptr; 

std::shared_ptr<GuiDescription> GuiDescription::instance = nullptr;
std::shared_ptr<Data> Data::instance = nullptr;
std::shared_ptr<InternalPlot> InternalPlot::instance = nullptr;
std::shared_ptr<EventPosition> EventPosition::instance = nullptr;
std::shared_ptr<LandmarkPool> LandmarkPool::instance = nullptr;
std::shared_ptr<CoordinateSystem> CoordinateSystem::instance = nullptr;
std::shared_ptr<EditMode> EditMode::instance = nullptr;
std::shared_ptr<PlotPosition> PlotPosition::instance = nullptr;

#endif
