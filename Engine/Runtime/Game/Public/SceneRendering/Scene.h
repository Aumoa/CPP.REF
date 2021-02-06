// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "SceneRendering/MinimalViewInfo.h"

class PrimitiveSceneProxy;
class PrimitiveComponent;
class APlayerController;
class SceneVisibility;
class LightComponent;
class LightSceneProxy;

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

	std::vector<LightComponent*> lightComponents;
	std::vector<LightSceneProxy*> lightProxies;

public:
	Scene();
	~Scene() override;

	void Update();
	void CalcVisibility();
	
	void AddScene(PrimitiveComponent* inPrimitiveComponent);
	void AddLight(LightComponent* inLightComponent);

	SceneVisibility* GetLocalPlayerVisibility() const;
	
	vs_property(APlayerController*, LocalPlayer);
	APlayerController* LocalPlayer_get() const;
	void LocalPlayer_set(APlayerController* value);

	vs_property_get_auto(std::span<PrimitiveComponent* const>, Primitives, primitiveComponents);
	vs_property_get_auto(std::span<PrimitiveSceneProxy* const>, PrimitiveSceneProxies, sceneProxies);
	vs_property_get_auto(std::span<LightComponent* const>, Lights, lightComponents);
	vs_property_get_auto(std::span<LightSceneProxy* const>, LightSceneProxies, lightProxies);
};