// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SLATECORE_API SViewport : public SCompoundWidget
{
public:
	SViewport();
	virtual ~SViewport() noexcept override;
	
public:
	BEGIN_SLATE_ATTRIBUTE(SViewport)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual Vector2 ComputeDesiredSize() const override;
};