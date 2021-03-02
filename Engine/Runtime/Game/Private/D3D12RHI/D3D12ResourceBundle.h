// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIResourceBundle.h"

#include "D3D12Minimal.h"

interface IRHIResource;
class D3D12DeviceBundle;

class D3D12ResourceBundle : virtual public Object, virtual public IRHIResourceBundle
{
public:
	using Super = Object;

private:
	D3D12DeviceBundle* deviceBundle;
	ID3D12Device5* device;

public:
	D3D12ResourceBundle();
	~D3D12ResourceBundle();

	virtual void InitializeBundle();
	virtual void ReleaseBundle();
	virtual void Commit(IRHICommandList* inCommandList);

	virtual TRefPtr<StaticMesh> CreateStaticMesh(const RHIStaticMeshGeometryData& inGeometryData);
	virtual TRefPtr<IRHIResource> CreateTexture2D(ERHITextureFormat format, PlatformImage* platformImage);
	virtual TRefPtr<IRHIScene> CreateScene(APlayerController* inPlayerController);

	TRefPtr<IRHIResource> CreateRaytracingScratchBuffer(uint64 sizeInBytes);
	TRefPtr<IRHIResource> CreateRaytracingAccelerationStructureBuffer(uint64 sizeInBytes);
};