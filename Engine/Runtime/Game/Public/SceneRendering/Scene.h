// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class SceneRenderer;
class PrimitiveSceneProxy;
class PrimitiveComponent;

class GAME_API Scene : virtual public Object
{
public:
	using Super = Object;
	using This = Scene;

	std::vector<PrimitiveComponent*> primitiveComponents;
	std::vector<PrimitiveSceneProxy*> sceneProxies;

public:
	Scene();
	~Scene() override;

	void Update();
	void Render(SceneRenderer* renderer);
	
	void AddScene(PrimitiveComponent* inPrimitiveComponent);
};