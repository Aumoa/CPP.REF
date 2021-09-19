// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"

class SWidget;
class SGameInstance;

class GAMELAUNCHER_API SUserWidget : public SGameObject
{
	GENERATED_BODY(SUserWidget)

private:
	SWidget* _buildedContent = nullptr;

public:
	SUserWidget();

	virtual SWidget* BuildSlateResources();
	virtual void ReleaseSlateResources();

	void AddWidgetToScreen();
	void RemoveFromParent();
};