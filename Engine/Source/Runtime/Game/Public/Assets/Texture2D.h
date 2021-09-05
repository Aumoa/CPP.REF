// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "StreamableRenderAsset.h"

class RHITexture2D;
class RHIShaderResourceView;

class GAME_API Texture2D : public StreamableRenderAsset
{
	GENERATED_BODY(Texture2D)

private:
	RHITexture2D* _texture = nullptr;
	RHIShaderResourceView* _shaderResourceView = nullptr;

public:
	Texture2D(const std::filesystem::path& assetPath);
	virtual ~Texture2D() override;

	virtual void StreamIn() override;

	RHITexture2D* GetRHITexture() const { return _texture; }
	RHIShaderResourceView* GetShaderResourceView() const { return _shaderResourceView; }
	void GetPixelSize(int32* pWidth, int32* pHeight);
};