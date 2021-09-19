// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Window.h"

class GAME_API SSlateApplication : public SWindow
{
	GENERATED_BODY(SSlateApplication)

public:
	SSlateApplication(const std::wstring& name);
	virtual ~SSlateApplication() override;

	void Init();

private:
	void OnMouse(Vector2N location, EMouseButton button, EMouseButtonEvent event);
	void OnKeyboard(EKey key, EKeyboardEvent event);
};