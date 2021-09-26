// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player.h"

class SGameEngine;
class SSlateApplication;
class SSlateShader;
class SRHIDeviceContext;
class SSlateWindowElementList;
struct IFrameworkView;

class GAME_API SLocalPlayer : public SPlayer
{
	GENERATED_BODY(SLocalPlayer)

private:
	IFrameworkView* _frameworkView = nullptr;
	SSlateApplication* _slateWindow = nullptr;
	SSlateWindowElementList* _drawElements = nullptr;

public:
	SLocalPlayer();

	void Init(IFrameworkView* frameworkView);
	void Tick(std::chrono::duration<float> elapsedTime);
	void Render(SRHIDeviceContext* deviceContext, SSlateShader* shader);

	inline IFrameworkView* GetFrameworkView() const { return _frameworkView; }
	inline SSlateApplication* GetSlateWindow() const { return _slateWindow; }

private:
	void OnResizedApp(int32 x, int32 y);
};