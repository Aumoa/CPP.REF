// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Transform.h"
#include "ComponentMobility.h"
#include "HomogeneousLight.h"

class GLightComponent;
class DirectXDynamicBuffer;

class GAME_API LightSceneProxy : virtual public Object
{
public:
	using Super = Object;
	using This = LightSceneProxy;

private:
	GLightComponent* myComponent;

public:
	LightSceneProxy(GLightComponent* inLightComponent);
	~LightSceneProxy() override;

	virtual void Update();
	virtual void UpdateTransform();

	GLightComponent* const Component;
	const EComponentMobility Mobility;
	Transform PrimitiveTransform;
	uint64 PrimitiveId;
	HomogeneousLight PrimitiveLight;
};