// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Materials/MaterialInterface.h"

interface IRHIShaderResourceView;
class RHIMaterialBundle;

class Material : public MaterialInterface
{
public:
	using Super = MaterialInterface;
	using This = Material;

private:
	bool bMarkDirty : 1;

	RHIMaterialBundle* owner;
	void* uploadBufferPtr;
	TRefPtr<IRHIShaderResourceView> textureGroupView;

public:
	Material(uint16 inMaterialIndex, RHIMaterialBundle* inBundle);
	~Material() override;

	void SetMarkDirty(EMaterialDirtyMask inAddMask) override;
	void ResolveDirtyState() override;

	IRHIShaderResourceView* SurfaceTextureSRV_get() const override;
};