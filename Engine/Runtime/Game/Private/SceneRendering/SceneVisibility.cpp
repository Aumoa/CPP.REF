// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneVisibility.h"

#include "Engine.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/Scene.h"
#include "Components/PrimitiveComponent.h"
#include "Shaders/ShaderTypes.h"

using namespace std;

SceneVisibility::SceneVisibility(Scene* inScene) : Super()
	, myScene(inScene)
	, bDirty(true)
	, cameraConstantPtr(nullptr)
{
	DirectXDeviceBundle* device = inScene->GetEngine()->GetDeviceBundle();
	DirectXAssign(cameraConstant, device->CreateDynamicBuffer(sizeof(ShaderCameraConstant)));
	HR(cameraConstant->Map(0, nullptr, &cameraConstantPtr));
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

		auto ptr = (ShaderCameraConstant*)cameraConstantPtr;
		ptr->ViewProj = myView.ViewProj;
		ptr->ViewProjInv = myView.ViewProjInv;
		ptr->Pos = myView.Location;

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

uint64 SceneVisibility::GetCameraConstantBuffer() const
{
	return cameraConstant->GetGPUVirtualAddress();
}