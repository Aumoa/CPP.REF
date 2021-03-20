// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Actor/RotateLight.h"

#include "Components/PointLightComponent.h"

ARotateLight::ARotateLight() : Super()
	, angle(0)
{
	PrimaryActorTick.bCanEverTick = true;

	LightComponent->Mobility = EComponentMobility::Movable;
	LightComponent->LightColor = Color::Red;
	LightComponent->Location = Vector3(0, 1, 0);
	LightComponent->Constant = 1;
	LightComponent->Linear = 0.35f;
	LightComponent->Quad = 0.44f;
}

ARotateLight::~ARotateLight()
{

}

void ARotateLight::TickActor(Seconds inDelta)
{
	Super::TickActor(inDelta);

	angle = (angle + 180.0_deg * inDelta.Value).Normalized;

	Vector3 location;
	auto [sin, cos] = Math::SinCos(angle);
	sin *= 2.0f;
	cos *= 2.0f;
	location.X = sin;
	location.Y = 0.5f;
	location.Z = cos;
	SetActorLocation(location);
}