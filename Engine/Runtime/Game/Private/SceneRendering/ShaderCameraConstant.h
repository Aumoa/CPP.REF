// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "SceneRendering/MinimalViewInfo.h"

interface IRHIResource;
class PrimitiveSceneProxy;

#pragma pack(push, 4)
struct alignas(256) ShaderObjectConstant
{
	Matrix4x4 World;
	Matrix4x4 WVP;
};

struct alignas(256) ShaderCameraConstant
{
	Matrix4x4 ViewProj;
	Matrix4x4 ViewProjInv;
	Vector3 Pos;
};
#pragma pack(pop)

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

	MinimalViewInfo currView;
	ShaderCameraConstant cameraConst;
	std::vector<ShaderObjectConstant> constants;

public:
	ShaderCameraConstantVector(size_t capacity = 0);
	~ShaderCameraConstantVector() override;

	void BeginUpdateConstant(const MinimalViewInfo& inView);
	void AddPrimitive(const PrimitiveSceneProxy* inPrimitive);
	void EndUpdateConstant();

	uint64 GetCameraConstantVirtualAddress() const;
	CameraConstantIterator GetBufferIterator() const;

private:
	static size_t AlignOf(size_t value);
};