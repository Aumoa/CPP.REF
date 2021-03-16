// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Transform.h"
#include "ComponentMobility.h"

class GLightComponent;
class DirectXDynamicBuffer;

class GAME_API LightSceneProxy : virtual public Object
{
public:
	using Super = Object;
	using This = LightSceneProxy;

private:
	GLightComponent* myComponent;
	TRefPtr<DirectXDynamicBuffer> lightShaderBuf;

public:
	LightSceneProxy(GLightComponent* inLightComponent, size_t inDesiredBuffSize = 0);
	~LightSceneProxy() override;

	virtual void Update();
	virtual void UpdateTransform();

	GLightComponent* const Component;
	const EComponentMobility Mobility;
	Transform PrimitiveTransform;
	uint64 InstanceCBV;

protected:
	void* GetLightShaderBuffer() const;
};