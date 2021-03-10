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
	return nullptr;
}

void GameViewport::SetViewportResolution_Internal(int32 x, int32 y)
{
	if (x == 0 || y == 0)
	{
		return;
	}

	resX = x;
	resY = y;
	compatibleTarget->ResizeBuffers(x, y);
}