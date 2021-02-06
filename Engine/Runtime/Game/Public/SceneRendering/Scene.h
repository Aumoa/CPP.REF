// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "SceneRendering/MinimalViewInfo.h"

class PrimitiveSceneProxy;
class PrimitiveComponent;
class APlayerController;
class SceneVisibility;

class GAME_API Scene : virtual public Object
{
public:
	using Super = Object;
	using This = Scene;

	std::vector<PrimitiveComponent*> primitiveComponents;
	std::vector<PrimitiveSceneProxy*> sceneProxies;

	APlayerController* localPlayer;
	TRefPtr<SceneVisibility> localPlayerVisibility;
	std::vector<TRefPtr<SceneVisibility>> visibilities;

public:
	Scene();
	~Scene() override;

	void Update();
	void CalcVisibility();
	
	void AddScene(PrimitiveComponent* inPrimitiveComponent);

	SceneVisibility* GetLocalPlayerVisibility() const;
	
	vs_property(APlayerController*, LocalPlayer);
	APlayerController* LocalPlayer_get() const;
	void LocalPlayer_set(APlayerController* value);

	vs_property_get(std::span<PrimitiveComponent* const>, Primitives);
	std::span<PrimitiveComponent* const> Primitives_get() const;
	vs_property_get(std::span<PrimitiveSceneProxy* const>, PrimitiveSceneProxies);
	std::span<PrimitiveSceneProxy* const> PrimitiveSceneProxies_get() const;
};