// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "StreamableRenderAsset.h"

class SRHITexture2D;
class SRHIShaderResourceView;

class GAME_API STexture2D : public SStreamableRenderAsset
{
	GENERATED_BODY(STexture2D)

private:
	SRHITexture2D* _texture = nullptr;
	SRHIShaderResourceView* _shaderResourceView = nullptr;

public:
	STexture2D(const std::filesystem::path& assetPath);
	virtual ~STexture2D() override;

	virtual void StreamIn() override;

	SRHITexture2D* GetRHITexture() const { return _texture; }
	SRHIShaderResourceView* GetShaderResourceView() const { return _shaderResourceView; }
	void GetPixelSize(int32* pWidth, int32* pHeight) const;
	Vector2 GetSize() const;
};