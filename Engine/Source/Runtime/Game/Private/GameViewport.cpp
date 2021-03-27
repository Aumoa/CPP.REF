// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameViewport.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXCompatibleRenderTarget.h"

using namespace std;

GameViewport::GameViewport(DirectXDeviceBundle* deviceBundle) : Super()
	, resX(0)
	, resY(0)
{
	DirectXNew(compatibleTarget, DirectXCompatibleRenderTarget, deviceBundle);
}

GameViewport::~GameViewport()
{

}

DirectXCompatibleRenderTarget* GameViewport::GetCompatibleRenderTarget() const
{
	return compatibleTarget.Get();
}

void GameViewport::SetResolution(int32 x, int32 y)
{
	resX = x;
	resY = y;

	if (x != 0 && y != 0)
	{
		compatibleTarget->ResizeBuffers(x, y);
		SetViewportResolution_Internal(x, y);
	}
}

void GameViewport::SetViewportResolution_Internal(int32 x, int32 y)
{

}