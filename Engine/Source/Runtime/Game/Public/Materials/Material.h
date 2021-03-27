// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Materials/MaterialInterface.h"

class DirectXDeviceBundle;
class DirectXDynamicBuffer;

class Material : public MaterialInterface
{
public:
	using Super = MaterialInterface;
	using This = Material;

private:
	bool bMarkDirty : 1;
	DirectXDeviceBundle* deviceBundle;
	TRefPtr<DirectXDynamicBuffer> materialBuffer;
	TRefPtr<DirectXShaderResourceView> surfaceSRV;

public:
	Material(DirectXDeviceBundle* deviceBundle);
	~Material() override;

	void SetMarkDirty(EMaterialDirtyMask inAddMask) override;
	void ResolveDirtyState() override;

	DirectXShaderResourceView* SurfaceTextureSRV_get() const override;
	uint64 BufferLocation_get() const override;
};