// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneVisibility.h"

#include "Logging/LogMacros.h"
#include "Framework/PlayerController.h"
#include "Components/PlayerCameraManager.h"
#include "Shaders/ShaderCameraConstant.h"
#include "SceneRendering/SceneRenderer.h"
#include "SceneRendering/MinimalViewInfo.h"

SceneVisibility::SceneVisibility(APlayerController* inPlayerController) : Super()
	, playerController(inPlayerController)
{
	if (inPlayerController == nullptr)
	{
		SE_LOG(LogEngine, Error, L"SceneVisibility must created with valid context that have function that calc minimal view info.");
		return;
	}

	cameraManager = inPlayerController->GetComponent<PlayerCameraManager>();
	if (cameraManager == nullptr)
	{
		SE_LOG(LogEngine, Error, L"PlayerController have not a player camera manager component. It is undefined behavior.");
		return;
	}

	cameraConstants = NewObject<ShaderCameraConstantVector>();
}

SceneVisibility::~SceneVisibility()
{

}

void SceneVisibility::CalcVisibility(SceneRenderer* renderer)
{
	size_t prim_count = renderer->GetPrimitiveCount();
	PrimitiveSceneProxy* const* prims = renderer->GetPrimitives();

	MinimalViewInfo viewInfo;
	cameraManager->CalcCameraView(viewInfo);
	cameraConstants->BeginUpdateConstant(viewInfo, prim_count);

	for (size_t i = 0; i < prim_count; ++i)
	{
		cameraConstants->AddPrimitive(prims[i]);
	}
}

bool SceneVisibility::IsValid_get() const
{
	return playerController != nullptr && cameraManager != nullptr;
}