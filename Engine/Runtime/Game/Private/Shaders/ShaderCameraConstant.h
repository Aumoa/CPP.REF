// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIResource;
struct MinimalViewInfo;
class PrimitiveSceneProxy;

struct alignas(256) ShaderCameraConstant
{
	Matrix4x4 World;
	Matrix4x4 ViewProj;
	Matrix4x4 WVP;
};

class ShaderCameraConstantVector : virtual public Object
{
public:
	using Super = Object;
	using This = ShaderCameraConstantVector;

private:
	static constexpr size_t CapaUnit = 16384 / sizeof(ShaderCameraConstant);  // 16KB
	size_t capacity;
	TRefPtr<IRHIResource> constantBuffer;
	void* mapped_addr;
	size_t seekpos;
	Matrix4x4 vp;

public:
	ShaderCameraConstantVector(size_t capacity = 0);
	~ShaderCameraConstantVector() override;

	void BeginUpdateConstant(const MinimalViewInfo& viewInfo, size_t primitiveCount);
	void AddPrimitive(PrimitiveSceneProxy* primitive);
	void EndUpdateConstant();

private:
	static size_t AlignOf(size_t value);
};