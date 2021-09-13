// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Scene/SceneVisibility.h"
#include "Scene/Scene.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "GameStructures.h"

SSceneVisibility::SSceneVisibility(SScene* owner) : Super()
	, _scene(owner)
{
}

void SSceneVisibility::CalcVisibility(const MinimalViewInfo& view)
{
	size_t cnt = _scene->_primitives.size();
	_visibilityBits.Init(true, cnt);

	// Do frustum culling.
	FrustumCull();

	// Primitive culling.
	_visibilityCnt = 0;
	for (ConstBitIterator bitIt(_visibilityBits); bitIt; ++bitIt)
	{
		RelativeBitReference relativeBit = bitIt.GetIndex();
		BitArray::BitReference bit = _visibilityBits.AccessCorrespondingBit(relativeBit);
		if (bit)
		{
			SPrimitiveSceneProxy* primitive = _scene->_primitives[relativeBit.BitIndex];
			if (primitive == nullptr)
			{
				bit = false;
				continue;
			}

			if (primitive->IsHiddenInGame())
			{
				bit = false;
				continue;
			}

			++_visibilityCnt;
		}
	}

	if (_visibilityCnt != 0)
	{
		ReadyBuffer(_visibilityCnt, false);

		constexpr Vector3 up = Vector3(0, 1, 0);
		constexpr Vector3 forward = Vector3(0, 0, 1);
		Matrix4x4 viewMatrix = Matrix4x4::LookToLH(view.Location, view.Rotation.RotateVector(forward), view.Rotation.RotateVector(up));
		constexpr Radians angle = Degrees(45.0f).ToRadians();
		Matrix4x4 projMatrix = Matrix4x4::PerspectiveFovLH(view.FOVAngle.ToRadians(), view.AspectRatio, view.NearPlane, view.FarPlane);
		Matrix4x4 vp = Matrix4x4::Multiply(viewMatrix, projMatrix);

		// Push camera constants.
		auto* ptr = (RHIViewConstants*)_viewBuffer->GetMappingPointer();
		for (ConstBitIterator bitIt(_visibilityBits); bitIt; ++bitIt)
		{
			RelativeBitReference relativeBit = bitIt.GetIndex();
			if (_visibilityBits.AccessCorrespondingBit(relativeBit))
			{
				SPrimitiveSceneProxy* primitive = _scene->_primitives[relativeBit.BitIndex];
				Transform transform = primitive->ComponentTransform;
				ptr->World = transform.GetMatrix();
				ptr->WorldViewProj = Matrix4x4::Multiply(ptr->World, vp);
				ptr->WorldInvTranspose = (Matrix4x4&)ptr->World.GetInverse().GetTransposed();
				++ptr;
			}
		}
	}
}

void SSceneVisibility::SetupView(SRHIDeviceContext* dc, SRHIShader* shader, size_t idx)
{
	std::vector<RHIShaderParameterElement> elements = shader->GetShaderParameterDeclaration();
	uint64 bufferLocation = _viewBuffer->GetGPUVirtualAddress() + sizeof(RHIViewConstants) * idx;
	
	for (size_t i = 0; i < elements.size(); ++i)
	{
		switch (elements[i].Type)
		{
		case ERHIShaderParameterType::ParameterCollection_CameraConstants:
			dc->SetGraphicsRootConstantBufferView((uint32)i, bufferLocation);
			break;
		}
	}
}

void SSceneVisibility::FrustumCull()
{
	for (ConstBitIterator bitIt(_visibilityBits); bitIt; ++bitIt)
	{
		// NOT IMPLEMENTED.
		const bool bFrustumCulled = false;
		if (bFrustumCulled)
		{
			_visibilityBits.AccessCorrespondingBit(bitIt.GetIndex()) = false;
		}
	}
}

void SSceneVisibility::ReadyBuffer(size_t capa, bool bAllowShrink)
{
	SRHIDevice* dev = _scene->GetDevice();
	size_t prev = sizeof(RHIViewConstants) * _viewBufCapa;
	size_t next = sizeof(RHIViewConstants) * capa;

	if (prev >= next)
	{
		// The view buffer not be allowed to shrink. Return immediately.
		return;
	}

	if (_viewBuffer != nullptr)
	{
		DestroySubobject(_viewBuffer);
	}

	_viewBuffer = dev->CreateDynamicBuffer(next);
	_viewBufCapa = capa;
}