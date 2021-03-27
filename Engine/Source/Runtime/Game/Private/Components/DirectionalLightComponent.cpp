// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/DirectionalLightComponent.h"

#include "SceneRendering/LightSceneProxy.h"
#include "Shaders/ShaderTypes.h"

class DirectionalLightSceneProxy : public LightSceneProxy
{
public:
	using Super = LightSceneProxy;

public:
	DirectionalLightSceneProxy(GDirectionalLightComponent* inComponent) : Super(inComponent)
	{
		Internal_UpdateTransform();
	}

	void UpdateTransform() override
	{
		Super::UpdateTransform();
		Internal_UpdateTransform();
	}

private:
	void Internal_UpdateTransform()
	{
		PrimitiveLight.LightIndex = HomogeneousLight::INDEX_DIRECTIONAL;
		PrimitiveLight.DirectionalLightAtt.Direction = PrimitiveTransform.Rotation.RotateVector(Vector3::Forward);
	}
};

GDirectionalLightComponent::GDirectionalLightComponent() : Super()
{

}

GDirectionalLightComponent::~GDirectionalLightComponent()
{

}

TRefPtr<LightSceneProxy> GDirectionalLightComponent::CreateSceneProxy()
{
	return NewObject<DirectionalLightSceneProxy>(this);
}