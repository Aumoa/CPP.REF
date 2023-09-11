// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SViewport.h"

class MAINFRAME_API SEditorViewport : public SViewport
{
public:
	SEditorViewport();

protected:
	virtual Vector2 ComputeDesiredSize() const { return Vector2::Zero(); }

	virtual int32 OnPaint([[maybe_unused]] const NPaintArgs& Args, [[maybe_unused]] const NGeometry& AllottedGeometry, [[maybe_unused]] const Rect& CullingRect, [[maybe_unused]] NSlateWindowElementList& OutDrawElements, int32 InLayer, [[maybe_unused]] bool bParentEnabled) const { return InLayer; }
};