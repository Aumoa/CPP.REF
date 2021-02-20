// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameViewport.h"

using namespace std;

GameViewport::GameViewport() : Super()
	, resX(0)
	, resY(0)
{

}

GameViewport::~GameViewport()
{

}

int32 GameViewport::ResolutionX_get() const
{
	return resX;
}

int32 GameViewport::ResolutionY_get() const
{
	return resY;
}

void GameViewport::SetViewportResolution_Internal(int32 x, int32 y)
{
	if (x == 0 || y == 0)
	{
		return;
	}

	resX = x;
	resY = y;
}