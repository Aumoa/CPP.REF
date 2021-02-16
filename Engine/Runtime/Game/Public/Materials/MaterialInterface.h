// Copyright 2020 Aumoa.lib. All right reserved.

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
	float Ambient_get() const;
	void Ambient_set(float value);

	vs_property(float, Diffuse);
	float Diffuse_get() const;
	void Diffuse_set(float value);

	vs_property(float, Specular);
	float Specular_get() const;
	void Specular_set(float value);

	vs_property(float, SpecExp);
	float SpecExp_get() const;
	void SpecExp_set(float value);

	vs_property(IRHIResource*, DiffuseMap);
	IRHIResource* DiffuseMap_get() const;
	void DiffuseMap_set(IRHIResource* value);

	vs_property(IRHIResource*, NormalMap);
	IRHIResource* NormalMap_get() const;
	void NormalMap_set(IRHIResource* value);

	vs_property_get_auto(uint16, Index, index);
	vs_property_get(IRHIShaderResourceView*, SurfaceTextureSRV);
	virtual IRHIShaderResourceView* SurfaceTextureSRV_get() const;
};

#include "MaterialInterface.inl"