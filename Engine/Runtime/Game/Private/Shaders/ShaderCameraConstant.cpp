// Copyright 2020 Aumoa.lib. All right reserved.

#include "ShaderCameraConstant.h"

#include "Engine.h"
#include "RHI/IRHIResource.h"
#include "RHI/IRHIDeviceBundle.h"
#include "SceneRendering/MinimalViewInfo.h"
#include "SceneRendering/PrimitiveSceneProxy.h"

ShaderCameraConstantVector::ShaderCameraConstantVector(size_t capacity) : Super()
	, capacity(0)
	, mapped_addr(nullptr)
	, seekpos(0)
{

}

ShaderCameraConstantVector::~ShaderCameraConstantVector()
{

}

void ShaderCameraConstantVector::BeginUpdateConstant(const MinimalViewInfo& viewInfo, size_t primitiveCount)
{
	if (primitiveCount == 0)
	{
		return;
	}

	size_t newsz = AlignOf(primitiveCount);
	if (newsz > this->capacity)
	{
		constantBuffer = GEngine.DeviceBundle->CreateDynamicConstantBuffer(newsz);
		this->capacity = newsz;
	}

	mapped_addr = constantBuffer->GetMappingAddress();
	seekpos = 0;

	Vector3 dir = viewInfo.Rotation.RotateVector(Vector3::Forward);
	Vector3 up = viewInfo.Rotation.RotateVector(Vector3::Up);
	vp = Matrix4x4::LookTo(viewInfo.Location, dir, up);
}

void ShaderCameraConstantVector::AddPrimitive(PrimitiveSceneProxy* primitive)
{
	ShaderCameraConstant* mapped_ptr = reinterpret_cast<ShaderCameraConstant*>(mapped_addr) + seekpos++;
	mapped_ptr->World = primitive->GetPrimitiveTransform().Matrix;
	mapped_ptr->ViewProj = vp;
	mapped_ptr->WVP = Matrix4x4::Multiply(mapped_ptr->World, mapped_ptr->ViewProj);
}

void ShaderCameraConstantVector::EndUpdateConstant()
{
	
}

size_t ShaderCameraConstantVector::AlignOf(size_t value)
{
	size_t newsz = (value + 255) & ~255;
	return newsz;
}