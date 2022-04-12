// Copyright 2020-2022 Aumoa.lib. All right reserved.

DEFINE_ASSEMBLY_INFO;

#include "SlotBase.h"

GENERATE_BODY(libty::SlateCore::Details::SSlotBase);

#include "Widgets/SWidget.h"
#include "Widgets/SLeafWidget.h"
#include "Widgets/SCompoundWidget.h"

GENERATE_BODY(libty::SlateCore::SWidget);
GENERATE_BODY(libty::SlateCore::SLeafWidget);
GENERATE_BODY(libty::SlateCore::SCompoundWidget);

#include "Widgets/Panel/SPanelWidget.h"
#include "Widgets/Panel/SBoxPanel.h"
#include "Widgets/Panel/SCanvasPanel.h"
#include "Widgets/Panel/SHorizontalBoxPanel.h"
#include "Widgets/Panel/SVerticalBoxPanel.h"

GENERATE_BODY(libty::SlateCore::SPanelWidget);
GENERATE_BODY(libty::SlateCore::SBoxPanel);
GENERATE_BODY(libty::SlateCore::SCanvasPanel);
GENERATE_BODY(libty::SlateCore::SHorizontalBoxPanel);
GENERATE_BODY(libty::SlateCore::SVerticalBoxPanel);

#include "Widgets/Text/STextBlock.h"

GENERATE_BODY(libty::SlateCore::STextBlock);

#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBox.h"

GENERATE_BODY(libty::SlateCore::SBorder);
GENERATE_BODY(libty::SlateCore::SScrollBox);

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/STextBox.h"

GENERATE_BODY(libty::SlateCore::SButton);
GENERATE_BODY(libty::SlateCore::STextBox);

#include "Widgets/Image/SImage.h"

GENERATE_BODY(libty::SlateCore::SImage);

#include "Renderer/SlateRenderer.h"
#include "Renderer/SlateRenderElementCollection.h"

GENERATE_BODY(libty::SlateCore::SSlateRenderer);
GENERATE_BODY(libty::SlateCore::SSlateRenderElementCollection);

#include "Layout/LayoutEnums.h"

GENERATE_BODY(libty::SlateCore::EFlowDirection);
GENERATE_BODY(libty::SlateCore::EOrientation);
GENERATE_BODY(libty::SlateCore::EWidgetClipping);
GENERATE_BODY(libty::SlateCore::EHorizontalAlignment);
GENERATE_BODY(libty::SlateCore::EVerticalAlignment);
GENERATE_BODY(libty::SlateCore::ESizeRule);
GENERATE_BODY(libty::SlateCore::ESlateVisibility);

#include "Draw/IRenderSlateElement.h"
#include "Draw/SlateBrush.h"
#include "Draw/SlateDrawCollector.h"

GENERATE_BODY(libty::SlateCore::IRenderSlateElement);
GENERATE_BODY(libty::SlateCore::SlateBrush);
GENERATE_BODY(libty::SlateCore::SSlateDrawCollector);
GENERATE_BODY(libty::SlateCore::SSlateDrawCollector::EElementType);
GENERATE_BODY(libty::SlateCore::SSlateDrawCollector::RenderElement);