// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Transform.h"
#include "ComponentMobility.h"
#include "DirectX/DirectXMinimal.h"
#include "DirectX/DirectXShaderBindingTable.h"

class GPrimitiveComponent;
class DirectXDynamicBuffer;

class GAME_API PrimitiveSceneProxy : public Object
{
public:
	using Super = Object;

private:
	TWeakPtr<GPrimitiveComponent> myPrimitiveComponent;
	TRefPtr<DirectXDynamicBuffer> instanceTransformBuf;

public:
	PrimitiveSceneProxy(GPrimitiveComponent* inPrimitiveComponent);
	~PrimitiveSceneProxy();

	virtual void Update();
	virtual void UpdateTransform();

	GPrimitiveComponent* const Component;
	const EComponentMobility Mobility;
	Transform PrimitiveTransform;
	uint64 PrimitiveId;
	AxisAlignedCube PrimitiveBoundingBox;
	uint64 PrimitiveAccelerationPtr;
	std::vector<DirectXInstanceShaderRecord> InstanceShaderRecord;

protected:
	uint64 GetInstanceTransformBuf() const;
};