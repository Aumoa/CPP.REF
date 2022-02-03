// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXResource.h"

class DIRECTX_API SDirectXTexture2D : public SDirectXResource, implements IRHITexture2D
{
	GENERATED_BODY(SDirectXTexture2D)

public:
	SDirectXTexture2D(IRHIDevice* Owner, ComPtr<ID3D12Resource> pResource);

	virtual Vector2N GetPixelSize() override;
};