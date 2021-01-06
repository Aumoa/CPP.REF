// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/ActorComponent.h"

#include "Logging/LogMacros.h"

using namespace std;
using namespace std::chrono;

ActorComponent::ComponentTickFunction::ComponentTickFunction() : Super()
	, Target(nullptr)
{

}

ActorComponent::ComponentTickFunction::~ComponentTickFunction()
{

}

void ActorComponent::ComponentTickFunction::ExecuteTick(duration<double> deltaTime)
{
	if (Target == nullptr)
	{
		SE_LOG(LogTicking, Error, L"Target is nullptr.");
		return;
	}

	if (Target->HasBegunPlay && Target->ComponentTickEnabled)
	{
		Target->TickComponent(deltaTime);
	}
}

ActorComponent::ActorComponent() : Super()
	, bComponentTickEnabled(true)
	, bComponentHasBegunPlay(false)
	, owner(nullptr)
{
	PrimaryComponentTick.Target = this;
}

ActorComponent::~ActorComponent()
{

}

void ActorComponent::BeginPlay()
{
	bComponentHasBegunPlay = true;
}

void ActorComponent::EndPlay()
{
	bComponentHasBegunPlay = false;
}

void ActorComponent::TickComponent(duration<double> deltaTime)
{

}

AActor* ActorComponent::GetOwner() const
{
	return owner;
}

auto ActorComponent::PrimaryComponentTick_get() -> ComponentTickFunction&
{
	return primaryComponentTick;
}

bool ActorComponent::ComponentTickEnabled_get() const
{
	return bComponentTickEnabled;
}

void ActorComponent::ComponentTickEnabled_set(bool value)
{
	bComponentTickEnabled = true;
}

bool ActorComponent::HasBegunPlay_get() const
{
	return bComponentHasBegunPlay;
}