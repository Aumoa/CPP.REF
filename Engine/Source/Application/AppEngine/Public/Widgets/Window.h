// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CompoundWidget.h"

interface IRHIDevice;
interface IApplicationInterface;

class APPENGINE_API SWindow : public SCompoundWidget
{
	GENERATED_BODY(SWindow)

private:
	IApplicationInterface* App = nullptr;

public:
	SWindow();

	virtual Vector2 GetDesiredSize() override;

	void TickAndPaint(float InDeltaTime, IRHIDevice* Device);

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(IApplicationInterface*, Application)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};