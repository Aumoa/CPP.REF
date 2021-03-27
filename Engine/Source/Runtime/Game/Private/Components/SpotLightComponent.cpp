// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/SpotLightComponent.h"

#include "SceneRendering/LightSceneProxy.h"
#include "Shaders/ShaderTypes.h"

class SpotLightSceneProxy : public LightSceneProxy
{
public:
	using Super = LightSceneProxy;

private:
	GSpotLightComponent* component;

public:
	SpotLightSceneProxy(GSpotLightComponent* inComponent) : Super(inComponent)
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
		PrimitiveLight.SpotLightAtt.CutOff = Math::Cos(component->CutOff);
		PrimitiveLight.SpotLightAtt.OuterCutOff = Math::Cos(component->OuterCutOff);
		PrimitiveLight.SpotLightAtt.Constant = component->Constant;
		PrimitiveLight.SpotLightAtt.Linear = component->Linear;
		PrimitiveLight.SpotLightAtt.Quad = component->Quad;
	}

	void Internal_UpdateTransform()
	{
		PrimitiveLight.LightIndex = HomogeneousLight::INDEX_SPOT;
		PrimitiveLight.SpotLightAtt.Direction = PrimitiveTransform.Rotation.RotateVector(Vector3::Forward);
		PrimitiveLight.SpotLightAtt.Position = PrimitiveTransform.Translation;
	}
};

GSpotLightComponent::GSpotLightComponent() : Super()
	, cutoff(12.5_deg)
	, outerCutoff(17.5_deg)
	, constant(1.0f)
	, linear(0.045f)
	, quad(0.0075f)
{
	Ambient = 0.05f;
}

GSpotLightComponent::~GSpotLightComponent()
{

}

TRefPtr<LightSceneProxy> GSpotLightComponent::CreateSceneProxy()
{
	return NewObject<SpotLightSceneProxy>(this);
}

Radians GSpotLightComponent::CutOff_get() const
{
	return cutoff;
}

void GSpotLightComponent::CutOff_set(Radians value)
{
	cutoff = value;
	SetMarkDirtyLightUpdated();
}

Radians GSpotLightComponent::OuterCutOff_get() const
{
	return outerCutoff;
}

void GSpotLightComponent::OuterCutOff_set(Radians value)
{
	outerCutoff = value;
	SetMarkDirtyLightUpdated();
}

float GSpotLightComponent::Constant_get() const
{
	return constant;
}

void GSpotLightComponent::Constant_set(float value)
{
	constant = value;
	SetMarkDirtyLightUpdated();
}

float GSpotLightComponent::Linear_get() const
{
	return linear;
}

void GSpotLightComponent::Linear_set(float value)
{
	linear = value;
	SetMarkDirtyLightUpdated();
}

float GSpotLightComponent::Quad_get() const
{
	return quad;
}

void GSpotLightComponent::Quad_set(float value)
{
	quad = value;
	SetMarkDirtyLightUpdated();
}