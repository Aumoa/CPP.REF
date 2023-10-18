// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/StreamableRenderAssetProxy.h"

class NStreamableRenderAssetProxy;
class NRHIGameShader;

struct RENDERCORE_API NRenderGlobal : public StaticClass
{
	static void Initialize();
	static void Shutdown();

	static const std::shared_ptr<NStreamableRenderAssetProxy>& GetNullRenderProxy() { return NullRenderProxy; }
	static const std::shared_ptr<NRHIGameShader>& GetDefaultGameShader() { return DefaultGameShader; }
	static std::shared_ptr<NStreamableRenderAssetProxy> LoadTexture2DAsync(String InFilename);

private:
	static std::shared_ptr<NStreamableRenderAssetProxy> NullRenderProxy;
	static std::shared_ptr<NRHIGameShader> DefaultGameShader;
};