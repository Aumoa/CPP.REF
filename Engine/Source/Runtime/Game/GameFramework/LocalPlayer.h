// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player.h"

class GameEngine;
class SWindow;
struct IFrameworkView;

class LocalPlayer : public Player
{
public:
	using Super = Player;

private:
	IFrameworkView* _frameworkView = nullptr;
	SWindow* _slateWindow = nullptr;

public:
	LocalPlayer();

	void Init(GameEngine* engine);
	void Tick(std::chrono::duration<float> elapsedTime);
	void Render();

private:
	void OnResizedApp(int32 x, int32 y);
};