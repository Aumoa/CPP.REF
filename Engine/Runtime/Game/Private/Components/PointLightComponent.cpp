// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/PointLightComponent.h"

#include "SceneRendering/LightSceneProxy.h"
#include "Shaders/ShaderTypes.h"

class PointLightSceneProxy : public LightSceneProxy
{
public:
	using Super = LightSceneProxy;

private:
	GPointLightComponent* component;

public:
	PointLightSceneProxy(GPointLightComponent* inComponent) : Super(inComponent)
		, component(inComponent)
	{
		Internal_Update();
		Internal_UpdateTransform();
	}

	void Update() override
	{
		Super::Update();
		Internal_Update();
	}

	void UpdateTransform() override
	{
		Super::UpdateTransform();
		Internal_UpdateTransform();
	}

private:
	void Internal_Update()
	{
		PrimitiveLight.PointLightAtt.Constant = component->Constant;
		PrimitiveLight.PointLightAtt.Linear = component->Linear;
		PrimitiveLight.PointLightAtt.Quad = component->Quad;
	}

	void Internal_UpdateTransform()
	{
		PrimitiveLight.LightIndex = HomogeneousLight::INDEX_POINT;
		PrimitiveLight.PointLightAtt.Position = PrimitiveTransform.Translation;
	}
};

GPointLightComponent::GPointLightComponent() : Super()
	, constant(1.0f)
	, linear(0.7f)
	, quad(1.8f)
{
	Ambient = 0;
}

GPointLightComponent::~GPointLightComponent()
{

}

TRefPtr<LightSceneProxy> GPointLightComponent::CreateSceneProxy()
{
	return NewObject<PointLightSceneProxy>(this);
}

float GPointLightComponent::Constant_get() const
{
	return constant;
}

void GPointLightComponent::Constant_set(float value)
{
	constant = value;
	SetMarkDirtyLightUpdated();
}

float GPointLightComponent::Linear_get() const
{
	return linear;
}

void GPointLightComponent::Linear_set(float value)
{
	linear = value;
	SetMarkDirtyLightUpdated();
}

float GPointLightComponent::Quad_get() const
{
	return quad;
}

void GPointLightComponent::Quad_set(float value)
{
	quad = value;
	SetMarkDirtyLightUpdated();
}