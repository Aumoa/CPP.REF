// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Materials/Material.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXDynamicBufferAllocator.h"
#include "DirectX/DirectXShaderResourceView.h"
#include "Shaders/ShaderTypes.h"

Material::Material(DirectXDeviceBundle* deviceBundle) : Super()
	, bMarkDirty(false)
	, deviceBundle(nullptr)
{
	SetMarkDirty(EMaterialDirtyMask::All);
	
	DirectXDynamicBufferAllocator* allocator = deviceBundle->GetOrCreateDynamicBufferAllocator(sizeof(ShaderTypes::Material));
	materialBuffer = NewObject<DirectXDynamicBuffer>(allocator);
	surfaceSRV = NewObject<DirectXShaderResourceView>(deviceBundle, 3);
	surfaceSRV->CreateConstantBufferView(0, materialBuffer->GetGPUVirtualAddress(), sizeof(ShaderTypes::Material));
}

Material::~Material()
{
	
}

void Material::SetMarkDirty(EMaterialDirtyMask inAddMask)
{
	Super::SetMarkDirty(inAddMask);

	if (!bMarkDirty)
	{
		bMarkDirty = true;
	}
}

void Material::ResolveDirtyState()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC nullDesc = { };
	nullDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	nullDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	nullDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	nullDesc.Texture2D.MipLevels = 1;

	if (HasDirtyMark(EMaterialDirtyMask::RenderState))
	{
		ShaderTypes::Material mat;
		mat.Ambient = Ambient;
		mat.Diffuse = Diffuse;
		mat.Specular = Specular;
		mat.SpecExp = SpecExp;
		mat.bDiffuseMap = DiffuseMap != nullptr;
		memcpy(materialBuffer->GetBufferPointer(), &mat, sizeof(mat));
	}

	if (HasDirtyMark(EMaterialDirtyMask::SurfaceTexture))
	{
		surfaceSRV->CreateShaderResourceView(1, DiffuseMap, DiffuseMap ? nullptr : &nullDesc);
		surfaceSRV->CreateShaderResourceView(2, NormalMap, NormalMap ? nullptr : &nullDesc);
	}

	Super::ResolveDirtyState();
	bMarkDirty = false;
}

DirectXShaderResourceView* Material::SurfaceTextureSRV_get() const
{
	return surfaceSRV.Get();
}

uint64 Material::BufferLocation_get() const
{
	return materialBuffer->GetGPUVirtualAddress();
}