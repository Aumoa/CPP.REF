// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneView.h"
#include "SceneRendering/SceneViewScope.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/PrimitiveSceneInfo.h"
#include "RHI/IRHIDevice.h"

SceneView::SceneView(SScene* InScene) : _MyScene(InScene)
{
}

void SceneView::InitViews(IRHIDeviceContext* Context, const SceneViewScope& InViewScope)
{
	const auto& Primitives = _MyScene->GetPrimitives();

	// Cleanup views.
	_Views.resize(0);
	ViewIndexes.resize(0);
	_Views.reserve(Primitives.size());
	ViewIndexes.reserve(Primitives.size());

	const Matrix4x4 ViewProj = Matrix4x4::Multiply(InViewScope.ViewMatrix, InViewScope.ProjMatrix);
	for (size_t i = 0; i < Primitives.size(); ++i)
	{
		const int64 PrimitiveId = (int64)i;
		const std::optional<PrimitiveSceneInfo>& PrimitiveInfo = Primitives[i];

		if (PrimitiveInfo.has_value() && PrimitiveInfo->bHiddenInGame)
		{
			Matrix4x4 World = PrimitiveInfo->ComponentTransform.GetMatrix();
			Matrix4x4 WorldViewProj = Matrix4x4::Multiply(World, ViewProj);

			ViewIndexes.emplace_back() =
			{
				.PrimitiveId = PrimitiveId,
			};

			_Views.emplace_back() =
			{
				.World = World,
				.WorldViewProj = WorldViewProj
			};
		}
	}

	if (_Views.size())
	{
		// Query view buffer and assign.
		size_t RequiredViewSize = sizeof(ViewInfo) * _Views.size();
		_ViewBuffer = _MyScene->QueryViewBuffer(RequiredViewSize);
		memcpy(_ViewBuffer.pSysMem, _Views.data(), sizeof(RequiredViewSize));
	}
}

SScene* SceneView::GetScene() const
{
	return _MyScene;
}

uint64 SceneView::GetStructuredBufferViewAddress() const
{
	if (_Views.size())
	{
		return _MyScene->GetActualGPUVirtualAddress(_ViewBuffer);
	}
	else
	{
		return 0;
	}
}