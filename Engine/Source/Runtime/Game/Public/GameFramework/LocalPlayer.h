// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "SlateMinimal.h"
#include "Player.h"

class GameEngine;
class SWindow;
class SlateShader;
struct IFrameworkView;

class GAME_API LocalPlayer : public Player
{
public:
	using Super = Player;

private:
	IFrameworkView* _frameworkView = nullptr;
	SWindow* _slateWindow = nullptr;
	SlateWindowElementList* _drawElements = nullptr;

public:
	LocalPlayer();

	void Init(GameEngine* engine);
	void Tick(std::chrono::duration<float> elapsedTime);
	void Render(RHIDeviceContext* deviceContext, SlateShader* shader);

private:
	void OnResizedApp(int32 x, int32 y);
};