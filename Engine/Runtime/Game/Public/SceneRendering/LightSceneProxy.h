// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Transform.h"
#include "ComponentMobility.h"

interface IRHIResource;

class GLightComponent;
struct RHILight;

class GAME_API LightBatch : virtual public Object
{
public:
	using Super = Object;
	using This = LightBatch;

private:
	TRefPtr<IRHIResource> basicLightBuffer;

public:
	LightBatch();
	~LightBatch() override;

	virtual TRefPtr<IRHIResource> GetLightBuffer() const;
};

class GAME_API LightSceneProxy : virtual public Object
{
public:
	using Super = Object;
	using This = LightSceneProxy;

private:
	TRefPtr<LightBatch> batch;
	GLightComponent* myLightComponent;
	Transform transform;

public:
	LightSceneProxy(GLightComponent* inLightComponent);
	~LightSceneProxy() override;

	void UpdateMovable();

	virtual Transform GetLightTransform() const;
	virtual LightBatch* GetLightBatch() const;
	virtual void UpdateBatchBuffer();
};