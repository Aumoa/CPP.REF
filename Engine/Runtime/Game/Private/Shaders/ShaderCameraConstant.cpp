// Copyright 2020 Aumoa.lib. All right reserved.

#include "ShaderCameraConstant.h"

#include "Engine.h"
#include "RHI/IRHIResource.h"
#include "RHI/IRHIDeviceBundle.h"
#include "SceneRendering/MinimalViewInfo.h"
#include "SceneRendering/PrimitiveSceneProxy.h"

CameraConstantIterator::CameraConstantIterator(size_t size, uint64 virtualAddress, size_t count)
	: size(size)
	, virtualAddress(virtualAddress)
	, seekpos(0)
	, count(count)
{

}

bool CameraConstantIterator::MoveNext()
{
	seekpos += 1;
	return seekpos < count;
}

uint64 CameraConstantIterator::Current() const
{
	return virtualAddress + (uint64)(size * seekpos);
}

ShaderCameraConstantVector::ShaderCameraConstantVector(size_t capacity) : Super()
	, capacity(0)
{

}

ShaderCameraConstantVector::~ShaderCameraConstantVector()
{

}

void ShaderCameraConstantVector::BeginUpdateConstant()
{
	constants.resize(0);
}

void ShaderCameraConstantVector::AddPrimitive(const MinimalViewInfo& inView, const PrimitiveSceneProxy* inPrimitive)
{
	Matrix4x4 w = inPrimitive->GetPrimitiveTransform().Matrix;
	Matrix4x4 vp = inView.ViewProj;

	ShaderCameraConstant& constant = constants.emplace_back();
	constant.World = w;
	constant.ViewProj = vp;
	constant.WVP = Matrix4x4::Multiply(w, vp);
}

void ShaderCameraConstantVector::EndUpdateConstant()
{
	size_t actSize = sizeof(ShaderCameraConstant) * constants.size();

	size_t newsz = AlignOf(actSize);
	if (newsz > this->capacity)
	{
		constantBuffer = GEngine.DeviceBundle->CreateDynamicConstantBuffer(newsz);
		this->capacity = newsz;
	}

	void* ptr = constantBuffer->GetMappingAddress();
	std::memcpy(ptr, constants.data(), actSize);
}

CameraConstantIterator ShaderCameraConstantVector::GetBufferIterator() const
{
	return CameraConstantIterator(sizeof(ShaderCameraConstant), constantBuffer->GetVirtualAddress(), constants.size());
}

size_t ShaderCameraConstantVector::AlignOf(size_t value)
{
	size_t newsz = (value + 255) & ~255;
	return newsz;
}