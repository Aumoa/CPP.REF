// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Materials/Material.h"

#include "Engine.h"

Material::Material(uint16 inMaterialIndex, RHIMaterialBundle* inBundle) : Super(inMaterialIndex)
	, bMarkDirty(false)
	, owner(inBundle)
	, uploadBufferPtr(nullptr)
{
	SetMarkDirty(EMaterialDirtyMask::All);
}

Material::~Material()
{
	//owner->ReleaseMaterial(Index);
}

void Material::SetMarkDirty(EMaterialDirtyMask inAddMask)
{
	Super::SetMarkDirty(inAddMask);

	if (!bMarkDirty)
	{
		//owner->EnqueueDirty(this);
		//bMarkDirty = true;
	}
}

void Material::ResolveDirtyState()
{
	if (HasDirtyMark(EMaterialDirtyMask::UpdateBuffer))
	{
		//uploadBufferPtr = owner->GetUploadBufferPointer(Index);
	}

	if (HasDirtyMark(EMaterialDirtyMask::RenderState))
	{
		//RHIMaterial mat = { Ambient, Diffuse, Specular, SpecExp };
		//memcpy(uploadBufferPtr, &mat, sizeof(mat));
	}

	if (HasDirtyMark(EMaterialDirtyMask::SurfaceTexture))
	{
		IRHIResource* ppResources[] = { DiffuseMap, NormalMap };
		//if (textureGroupView.IsValid)
		{
			//GEngine.DeviceBundle->UpdateTextureGroupView(textureGroupView.Get(), ppResources);
		}
		//else
		{
			//textureGroupView = GEngine.DeviceBundle->CreateTextureGroupView(ppResources);
		}
	}

	Super::ResolveDirtyState();
	bMarkDirty = false;
}

IRHIShaderResourceView* Material::SurfaceTextureSRV_get() const
{
	//return textureGroupView.Get();
	return nullptr;
}