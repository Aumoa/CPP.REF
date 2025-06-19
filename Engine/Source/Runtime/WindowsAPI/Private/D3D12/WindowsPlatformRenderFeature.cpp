// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12/WindowsPlatformRenderFeature.h"
#include "D3D12/D3D12Graphics.h"

namespace Ayla
{
	RPtr<Graphics> WindowsPlatformRenderFeature::CreateGraphics(RenderAPI api)
	{
		switch (api)
		{
		case RenderAPI::D3D12:
			return New<D3D12Graphics>();
		default:
			Debug::LogErrorFormat(TEXT("LogWindows"), TEXT("The specified Render API({0}) is not supported on the windows platform."), api);
			throw new InvalidOperationException();
		}
	}
}