#ifndef __STATIC_INITIALIZATIONS_H__
#define __STATIC_INITIALIZATIONS_H__

#include <memory>
#include "singleton/GuiDescription.h"
#include "singleton/InternalPlot.h"
#include "singleton/EventPosition.h"
#include "singleton/CoordinateSystem.h"
#include "observer/Observatory.h"
#include "observer/OpenScanButtonObserver.h"
#include "observer/OpenScanDialogObserver.h"
#include "observer/OpenScanButtonObserver.h"
#include "observer/ImageStackObserver.h"
#include "observer/DrawingAreaObserver.h"
#include "observer/ScrollbarObserver.h"
#include "observer/ViewModeObserver.h"
#include "observer/SliceObserver.h"
#include "observer/InternalPlotObserver.h"
#include "observer/ZoomObserver.h"
#include "observer/ErrorMessageDialogObserver.h"
#include "observer/CropObserver.h"
#include "observer/SaveCropSpecificationButtonObserver.h"
#include "observer/SaveCropSpecificationDialogObserver.h"

template<> std::shared_ptr<OpenScanButtonObserver>
Observatory<OpenScanButtonObserver>::observer = nullptr;

template<> std::shared_ptr<OpenScanDialogObserver>
Observatory<OpenScanDialogObserver>::observer = nullptr;

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

template<> std::shared_ptr<ErrorMessageDialogObserver>
Observatory<ErrorMessageDialogObserver>::observer = nullptr;

template<> std::shared_ptr<CropObserver>
Observatory<CropObserver>::observer = nullptr;

template<> std::shared_ptr<SaveCropSpecificationButtonObserver>
Observatory<SaveCropSpecificationButtonObserver>::observer = nullptr;

template<> std::shared_ptr<SaveCropSpecificationDialogObserver>
Observatory<SaveCropSpecificationDialogObserver>::observer = nullptr;

std::shared_ptr<GuiDescription> GuiDescription::instance = nullptr;
std::shared_ptr<Data> Data::instance = nullptr;
std::shared_ptr<InternalPlot> InternalPlot::instance = nullptr;
std::shared_ptr<EventPosition> EventPosition::instance = nullptr;
std::shared_ptr<CoordinateSystem> CoordinateSystem::instance = nullptr;
std::shared_ptr<PlotPosition> PlotPosition::instance = nullptr;

#endif
