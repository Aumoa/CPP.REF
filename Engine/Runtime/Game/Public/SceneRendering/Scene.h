// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "SceneRendering/MinimalViewInfo.h"

class SceneRenderer;
class PrimitiveSceneProxy;
class PrimitiveComponent;
class SceneVisibility;
class APlayerController;

class GAME_API Scene : virtual public Object
{
public:
	using Super = Object;
	using This = Scene;

	std::vector<PrimitiveComponent*> primitiveComponents;
	std::vector<PrimitiveSceneProxy*> sceneProxies;

	APlayerController* localPlayer;
	TRefPtr<SceneVisibility> localPlayerVisibility;

public:
	Scene();
	~Scene() override;

	void Update();
	void Render(SceneRenderer* renderer);
	
	void AddScene(PrimitiveComponent* inPrimitiveComponent);
	
	vs_property(APlayerController*, LocalPlayer);
	APlayerController* LocalPlayer_get() const;
	void LocalPlayer_set(APlayerController* value);
};