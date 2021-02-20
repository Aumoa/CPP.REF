// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneVisibility.h"

#include "SceneRendering/Scene.h"
#include "SceneRendering/ShaderCameraConstant.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "Components/PrimitiveComponent.h"

using namespace std;

SceneVisibility::SceneVisibility(Scene* inScene) : Super()
	, myScene(inScene)
	, bDirty(true)
{
	shaderCameraConstants = NewObject<ShaderCameraConstantVector>();
}

SceneVisibility::~SceneVisibility()
{

}

void SceneVisibility::CalcVisibility()
{
	if (bDirty)
	{
		const auto& primitives = myScene->Primitives;

		visibilities.resize(primitives.size());
		shaderCameraConstants->BeginUpdateConstant(myView);
		numPrimitivesRender = 0;

		for (size_t i = 0; i < primitives.size(); ++i)
		{
			PrimitiveSceneProxy* proxy = primitives[i]->GetSceneProxy();
			if (proxy == nullptr)
			{
				visibilities[i] = false;
				continue;
			}

			if (const AxisAlignedCube* boundingBoxEnabled = proxy->GetPrimitiveBoundingBox(); boundingBoxEnabled != nullptr)
			{
				AxisAlignedCube boundingBox = *boundingBoxEnabled;
				if (!OverlapTest::IsOverlap(viewFrustum, boundingBox))
				{
					// Skip this primitive.
					visibilities[i] = false;
					continue;
				}
			}

			shaderCameraConstants->AddPrimitive(proxy);
			visibilities[i] = true;
			numPrimitivesRender += 1;
		}

		shaderCameraConstants->EndUpdateConstant();

		bDirty = false;
	}
}

void SceneVisibility::UpdateView(const MinimalViewInfo& inView)
{
	myView = inView;
	bDirty = true;

	viewFrustum = Frustum::Construct(inView.ViewProj);
}

const vector<bool>& SceneVisibility::PrimitiveVisibility_get() const
{
	return visibilities;
}

ShaderCameraConstantVector* SceneVisibility::ShaderCameraConstants_get() const
{
	return shaderCameraConstants.Get();
}