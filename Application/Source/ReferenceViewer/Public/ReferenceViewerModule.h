// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApplicationModule.h"

class SWindow;

class REFERENCEVIEWER_API SReferenceViewerModule : public SApplicationModule
{
	GENERATED_BODY(SReferenceViewerModule)

public:
	virtual SWindow* CreateWindow() override;
};