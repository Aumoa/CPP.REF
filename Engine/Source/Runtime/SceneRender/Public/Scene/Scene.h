// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PrimitiveSceneInfo.h"
#include "SceneView/SceneView.h"
#include "SceneView/SceneViewScope.h"
#include "SceneRendering/SceneRenderTarget.h"

interface IRHIDevice;
interface IRHIDeviceContext;
class SceneView;
class SceneRenderTarget;

class SCENERENDER_API SScene : implements SObject
{
	GENERATED_BODY(SScene)

private:
	IRHIDevice* _Device = nullptr;
	SceneView _PrimarySceneView;

public:
	std::vector<PrimitiveSceneInfo> Primitives;

public:
	SScene(IRHIDevice* InDevice);
	~SScene() override;
	
	void InitViews(IRHIDeviceContext* Context, const SceneViewScope& InPrimarySceneView);
	void PopulateCommandLists(IRHIDeviceContext* Context, const SceneRenderTarget& InRenderTarget);

	IRHIDevice* GetDevice();
};