// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXDeviceChild.h"

class DIRECTX_API SDirectXResource : public SDirectXDeviceChild, implements IRHIResource
{
	GENERATED_BODY(SDirectXResource)

public:
	ComPtr<ID3D12Resource> pResource;

public:
	SDirectXResource(IRHIDevice* Owner, ComPtr<ID3D12Resource> pResource);

	using Super::Dispose;

protected:
	virtual void Dispose(bool bDisposing) override;
};