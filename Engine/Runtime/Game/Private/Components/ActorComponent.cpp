// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/ActorComponent.h"

#include "Logging/LogMacros.h"

using namespace std;

ActorComponent::ComponentTickFunction::ComponentTickFunction() : Super()
	, Target(nullptr)
{

}

ActorComponent::ComponentTickFunction::~ComponentTickFunction()
{

}

void ActorComponent::ComponentTickFunction::ExecuteTick(Seconds deltaTime)
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

TickFunction* ActorComponent::GetTickFunction()
{
	return &PrimaryComponentTick;
}

void ActorComponent::AddPrerequisiteObject(ITickFunctionObject* inObject)
{
	PrimaryComponentTick.AddPrerequisite(inObject);
}

void ActorComponent::RemovePrerequisiteObject(ITickFunctionObject* inObject)
{
	PrimaryComponentTick.RemovePrerequisite(inObject);
}

void ActorComponent::BeginPlay()
{
	bComponentHasBegunPlay = true;
}

void ActorComponent::EndPlay()
{
	bComponentHasBegunPlay = false;
}

void ActorComponent::TickComponent(Seconds deltaTime)
{

}

AActor* ActorComponent::GetOwner() const
{
	return owner;
}

auto ActorComponent::PrimaryComponentTick_get() const -> ComponentTickFunction&
{
	return const_cast<ComponentTickFunction&>(primaryComponentTick);
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