// Copyright 2020-2021 Aumoa.lib. All right reserved.

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

void ShaderCameraConstantVector::BeginUpdateConstant(const MinimalViewInfo& inView)
{
	constants.resize(0);
	
	cameraConst.ViewProj = inView.ViewProj;
	cameraConst.ViewProjInv = inView.ViewProjInv;
	cameraConst.Pos = inView.Location;

	currView = inView;
}

void ShaderCameraConstantVector::AddPrimitive(const PrimitiveSceneProxy* inPrimitive)
{
	Matrix4x4 w = inPrimitive->GetPrimitiveTransform().Matrix;
	Matrix4x4 vp = currView.ViewProj;

	ShaderObjectConstant& constant = constants.emplace_back();
	constant.World = w;
	constant.WVP = Matrix4x4::Multiply(w, vp);
}

void ShaderCameraConstantVector::EndUpdateConstant()
{
	constexpr size_t CameraConst = sizeof(ShaderCameraConstant);
	size_t actSize = sizeof(ShaderObjectConstant) * constants.size();

	size_t newsz = AlignOf(CameraConst + actSize);
	if (newsz > this->capacity)
	{
		constantBuffer = GEngine.DeviceBundle->CreateDynamicBuffer(newsz);
		this->capacity = newsz;
	}

	char* ptr = (char*)constantBuffer->GetMappingAddress();
	memcpy(ptr, &cameraConst, CameraConst);
	memcpy(ptr + CameraConst, constants.data(), actSize);
}

uint64 ShaderCameraConstantVector::GetCameraConstantVirtualAddress() const
{
	return constantBuffer->GetVirtualAddress();
}

CameraConstantIterator ShaderCameraConstantVector::GetBufferIterator() const
{
	constexpr size_t CameraConst = sizeof(ShaderCameraConstant);
	return CameraConstantIterator(sizeof(ShaderCameraConstant), constantBuffer->GetVirtualAddress() + CameraConst, constants.size() - CameraConst);
}

size_t ShaderCameraConstantVector::AlignOf(size_t value)
{
	size_t newsz = (value + 255) & ~255;
	return newsz;
}