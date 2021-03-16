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
		if (Mobility == EComponentMobility::Static)
		{
			Internal_UpdateTransform();
		}
	}

	void UpdateTransform() override
	{
		Super::UpdateTransform();
		Internal_UpdateTransform();
	}

private:
	void Internal_UpdateTransform()
	{
		auto* ptr = (ShaderTypes::GeneralLight*)GetLightShaderBuffer();
		ptr->Type = LightType_Directional;
		ptr->DirectionalLight_Direction = PrimitiveTransform.Rotation.RotateVector(Vector3::Forward).Cast<Vector4>();
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