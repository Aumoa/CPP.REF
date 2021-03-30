// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "DirectX/DirectXMinimal.h"

class DirectXShaderResourceView;
class DirectXDynamicBuffer;

enum class EMaterialDirtyMask : uint8
{
	None = 0,
	RenderState = 1,
	SurfaceTexture = RenderState << 1,
	All = 0xFF
};

class GAME_API MaterialInterface : virtual public Object
{
	MaterialInterface(const MaterialInterface&) = delete;

public:
	using Super = Object;
	using This = MaterialInterface;

private:
	EMaterialDirtyMask dirtyMask;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	float specExp;

	TComPtr<ID3D12Resource> diffuseMap;
	TComPtr<ID3D12Resource> normalMap;

public:
	MaterialInterface();
	~MaterialInterface() override;

	virtual void SetMarkDirty(EMaterialDirtyMask inAddMask);
	virtual void ResolveDirtyState();
	bool HasAnyDirtyMark() const;
	bool HasDirtyMark(EMaterialDirtyMask inCompareMask) const;

	vs_property(Vector3, Ambient);
	vs_property(Vector3, Diffuse);
	vs_property(Vector3, Specular);
	vs_property(float, SpecExp);

	vs_property(ID3D12Resource*, DiffuseMap);
	vs_property(ID3D12Resource*, NormalMap);

	vs_property_get_pure(DirectXShaderResourceView*, SurfaceTextureSRV);
	vs_property_get_pure(uint64, BufferLocation);
};

#include "MaterialInterface.inl"