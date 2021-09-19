// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"

class SBorder;

class GAMELAUNCHER_API SMainFrame : public SUserWidget
{
	GENERATED_BODY(SMainFrame)

private:
	SWidget* _content = nullptr;

public:
	SMainFrame();

	virtual SWidget* BuildSlateResources() override;
	virtual void ReleaseSlateResources() override;

private:
	SBorder* MakeSlot(std::wstring_view image, std::wstring_view name);
};