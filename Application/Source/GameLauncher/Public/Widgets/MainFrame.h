// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"

class SHorizontalBoxPanel;
class SBorder;

class GAMELAUNCHER_API SMainFrame : public SUserWidget
{
	GENERATED_BODY(SMainFrame)

private:
	SHorizontalBoxPanel* _content = nullptr;

public:
	SMainFrame();

	virtual SWidget* BuildSlateResources() override;
	virtual void ReleaseSlateResources() override;

private:
	SBorder* MakeSlot(const std::filesystem::path& image, std::wstring_view name, const std::filesystem::path& redirect);
};