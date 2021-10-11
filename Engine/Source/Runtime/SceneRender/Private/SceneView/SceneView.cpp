// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneView/SceneView.h"
#include "SceneView/SceneViewScope.h"
#include "Scene/Scene.h"
#include "Scene/PrimitiveSceneInfo.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIBuffer.h"
#include "RHI/IRHIDeviceContext.h"

SceneView::SceneView(SScene* InScene) : _MyScene(InScene)
{
}

void SceneView::InitViews(IRHIDeviceContext* Context, const SceneViewScope& InViewScope)
{
	// Cleanup views.
	Views.resize(0);
	ViewIndexes.resize(0);
	Views.reserve(_MyScene->Primitives.size());
	ViewIndexes.reserve(_MyScene->Primitives.size());

	const Matrix4x4 ViewProj = Matrix4x4::Multiply(InViewScope.ViewMatrix, InViewScope.ProjMatrix);
	for (auto& PrimitiveInfo : _MyScene->Primitives)
	{
		if (PrimitiveInfo.bHiddenInGame)
		{
			Matrix4x4 World = PrimitiveInfo.ComponentTransform.GetMatrix();
			Matrix4x4 WorldViewProj = Matrix4x4::Multiply(World, ViewProj);

			ViewIndexes.emplace_back() =
			{
				.PrimitiveId = PrimitiveInfo.PrimitiveId,
			};

			Views.emplace_back() =
			{
				.World = World,
				.WorldViewProj = WorldViewProj
			};
		}
	}

	// Reallocating buffer if size need to expand.
	if (_CurrentViewSize < Views.size() && Views.size() != 0)
	{
		IRHIDevice* Device = _MyScene->GetDevice();

		if (_ViewBuffer)
		{
			Context->PendingGarbageObject(_ViewBuffer);
			_ViewBuffer = nullptr;
		}

		RHIBufferDesc BufDesc =
		{
			.ByteWidth = (uint32)(sizeof(ViewInfo) * Views.size()),
			.Usage = ERHIBufferUsage::Default,
			.InitialState = ERHIResourceStates::NonPixelShaderResource,
			.StructureByteStride = sizeof(ViewInfo)
		};

		_ViewBuffer = Device->CreateBuffer(BufDesc, nullptr);
	}

	if (_ViewBuffer && Views.size() != 0)
	{
		// Update view buffer.
		RHISubresourceData SubresourceData =
		{
			.pSysMem = Views.data(),
			.SysMemPitch = sizeof(ViewInfo) * Views.size()
		};
		Context->UpdateSubresource(_ViewBuffer, 0, SubresourceData);
	}
}

SScene* SceneView::GetScene() const
{
	return _MyScene;
}

uint64 SceneView::GetStructuredBufferViewAddress() const
{
	if (_ViewBuffer)
	{
		return _ViewBuffer->GetGPUVirtualAddress();
	}
	else
	{
		return 0;
	}
}