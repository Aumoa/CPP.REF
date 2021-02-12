// Copyright 2020 Aumoa.lib. All right reserved.

#include "Materials/Material.h"

#include "RHI/RHICommon.h"
#include "RHI/RHIMaterialBundle.h"

Material::Material(uint16 inMaterialIndex, RHIMaterialBundle* inBundle) : Super(inMaterialIndex)
	, bMarkDirty(false)
	, owner(inBundle)
	, uploadBufferPtr(nullptr)
{
	SetMarkDirty(EMaterialDirtyMask::All);
}

Material::~Material()
{

}

void Material::SetMarkDirty(EMaterialDirtyMask inAddMask)
{
	Super::SetMarkDirty(inAddMask);

	if (!bMarkDirty)
	{
		owner->EnqueueDirty(this);
		bMarkDirty = true;
	}
}

void Material::ResolveDirtyState()
{
	if (HasDirtyMark(EMaterialDirtyMask::UpdateBuffer))
	{
		uploadBufferPtr = owner->GetUploadBufferPointer(Index);
	}

	if (HasDirtyMark(EMaterialDirtyMask::RenderState))
	{
		RHIMaterial mat = { Ambient, Diffuse, Specular, SpecExp };
		memcpy(uploadBufferPtr, &mat, sizeof(mat));
	}

	Super::ResolveDirtyState();
	bMarkDirty = false;
}