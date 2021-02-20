// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Materials/MaterialInterface.h"

#include "RHI/IRHIResource.h"

MaterialInterface::MaterialInterface(uint16 inIndex) : Super()
	, index(inIndex)
	, dirtyMask(EMaterialDirtyMask::All)
	, ambient(1.0f)
	, diffuse(1.0f)
	, specular(1.0f)
	, specExp(32.0f)
{

}

MaterialInterface::~MaterialInterface()
{

}

void MaterialInterface::SetMarkDirty(EMaterialDirtyMask inAddMask)
{
	dirtyMask |= inAddMask;
}

void MaterialInterface::ResolveDirtyState()
{
	dirtyMask = EMaterialDirtyMask::None;
}

bool MaterialInterface::HasAnyDirtyMark() const
{
	return dirtyMask != EMaterialDirtyMask::None;
}

bool MaterialInterface::HasDirtyMark(EMaterialDirtyMask inCompareMask) const
{
	return (dirtyMask & inCompareMask) != EMaterialDirtyMask::None;
}

float MaterialInterface::Ambient_get() const
{
	return ambient;
}

void MaterialInterface::Ambient_set(float value)
{
	if (ambient != value)
	{
		ambient = value;
		SetMarkDirty(EMaterialDirtyMask::RenderState);
	}
}

float MaterialInterface::Diffuse_get() const
{
	return diffuse;
}

void MaterialInterface::Diffuse_set(float value)
{
	if (diffuse != value)
	{
		diffuse = value;
		SetMarkDirty(EMaterialDirtyMask::RenderState);
	}
}

float MaterialInterface::Specular_get() const
{
	return specular;
}

void MaterialInterface::Specular_set(float value)
{
	if (specular != value)
	{
		specular = value;
		SetMarkDirty(EMaterialDirtyMask::RenderState);
	}
}

float MaterialInterface::SpecExp_get() const
{
	return specExp;
}

void MaterialInterface::SpecExp_set(float value)
{
	if (specExp != value)
	{
		specExp = value;
		SetMarkDirty(EMaterialDirtyMask::RenderState);
	}
}

IRHIResource* MaterialInterface::DiffuseMap_get() const
{
	return diffuseMap.Get();
}

void MaterialInterface::DiffuseMap_set(IRHIResource* value)
{
	if (diffuseMap.Get() != value)
	{
		diffuseMap = value;
		SetMarkDirty(EMaterialDirtyMask::SurfaceTexture);
	}
}

IRHIResource* MaterialInterface::NormalMap_get() const
{
	return normalMap.Get();
}

void MaterialInterface::NormalMap_set(IRHIResource* value)
{
	if (normalMap.Get() != value)
	{
		normalMap = value;
		SetMarkDirty(EMaterialDirtyMask::SurfaceTexture);
	}
}

IRHIShaderResourceView* MaterialInterface::SurfaceTextureSRV_get() const
{
	return nullptr;
}