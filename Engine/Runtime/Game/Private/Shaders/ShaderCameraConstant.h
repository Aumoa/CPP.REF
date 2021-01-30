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

struct CameraConstantIterator
{
private:
	size_t size;
	uint64 virtualAddress;

	size_t seekpos;
	size_t count;

public:
	CameraConstantIterator(size_t size, uint64 virtualAddress, size_t count);

	bool MoveNext();
	uint64 Current() const;
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

	std::vector<ShaderCameraConstant> constants;

public:
	ShaderCameraConstantVector(size_t capacity = 0);
	~ShaderCameraConstantVector() override;

	void BeginUpdateConstant();
	void AddPrimitive(const MinimalViewInfo& inView, const PrimitiveSceneProxy* inPrimitive);
	void EndUpdateConstant();

	CameraConstantIterator GetBufferIterator() const;

private:
	static size_t AlignOf(size_t value);
};