// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneVisibility.h"

#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/Scene.h"
#include "Components/PrimitiveComponent.h"

using namespace std;

SceneVisibility::SceneVisibility(Scene* inScene) : Super()
	, myScene(inScene)
	, bDirty(true)
{

}

SceneVisibility::~SceneVisibility()
{

}

void SceneVisibility::CalcVisibility()
{
	if (bDirty)
	{
		const auto& primitives = myScene->GetPrimitives();

		visibilities.resize(primitives.size());
		numPrimitivesRender = 0;

		for (size_t i = 0; i < primitives.size(); ++i)
		{
			PrimitiveSceneProxy* proxy = primitives[i];
			if (proxy == nullptr)
			{
				visibilities[i] = false;
				continue;
			}

			AxisAlignedCube boundingBox = proxy->PrimitiveBoundingBox;
			if (!OverlapTest::IsOverlap(viewFrustum, boundingBox))
			{
				// Skip this primitive.
				visibilities[i] = false;
				continue;
			}

			visibilities[i] = true;
			numPrimitivesRender += 1;
		}

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