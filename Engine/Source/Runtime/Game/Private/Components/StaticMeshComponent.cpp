// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Components/StaticMeshComponent.h"
#include "Rendering/PrimitiveSceneProxy.h"
#include "StaticMeshComponent.gen.cpp"

class StaticMeshSceneProxy : public PrimitiveSceneProxy
{
public:
	StaticMeshSceneProxy()
	{
	}

	virtual void DrawStaticElements(StaticPrimitiveDrawingInterface* PDI)
	{
	}
};

PrimitiveSceneProxy* StaticMeshComponent::CreateSceneProxy()
{
	return new StaticMeshSceneProxy();
}