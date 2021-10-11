// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext;
class SceneView;

class SCENERENDER_API SceneRenderer
{
	SceneView* _View = nullptr;

public:
	SceneRenderer(SceneView* InView);

	void PopulateCommandLists(IRHIDeviceContext* Context);
};