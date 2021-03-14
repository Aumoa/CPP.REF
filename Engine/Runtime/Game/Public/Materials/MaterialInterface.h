// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIResource;
interface IRHIShaderResourceView;

enum class EMaterialDirtyMask : uint8
{
	None = 0,
	RenderState = 1,
	UpdateBuffer = RenderState << 1,
	SurfaceTexture = UpdateBuffer << 1,
	All = 0xFF
};

class GAME_API MaterialInterface : virtual public Object
{
public:
	using Super = Object;
	using This = MaterialInterface;

private:
	uint16 index;
	EMaterialDirtyMask dirtyMask;
	float ambient;
	float diffuse;
	float specular;
	float specExp;
	
	TRefPtr<IRHIResource> diffuseMap;
	TRefPtr<IRHIResource> normalMap;

public:
	MaterialInterface(uint16 inIndex);
	~MaterialInterface() override;

	virtual void SetMarkDirty(EMaterialDirtyMask inAddMask);
	virtual void ResolveDirtyState();
	bool HasAnyDirtyMark() const;
	bool HasDirtyMark(EMaterialDirtyMask inCompareMask) const;

	vs_property(float, Ambient);
	vs_property(float, Diffuse);
	vs_property(float, Specular);
	vs_property(float, SpecExp);

	vs_property(IRHIResource*, DiffuseMap);
	vs_property(IRHIResource*, NormalMap);

	vs_property_get_auto(uint16, Index, index);
	vs_property_get_virtual(IRHIShaderResourceView*, SurfaceTextureSRV);
};

#include "MaterialInterface.inl"