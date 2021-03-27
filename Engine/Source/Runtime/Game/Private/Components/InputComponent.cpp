// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/InputComponent.h"

#include "Logging/LogMacros.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "Windows/WinKeyboard.h"
#include "Windows/WinMouse.h"

using namespace std;

DEFINE_STATS_GROUP(GInputComponent);

GInputComponent::GInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	keyTracker = NewObject<KeyStateTracker>();
	mouseTracker = NewObject<MouseStateTracker>();
}

GInputComponent::~GInputComponent()
{

}

void GInputComponent::TickComponent(Seconds deltaTime)
{
	Super::TickComponent(deltaTime);

	keyTracker->Update(WinKeyboard::Get().GetState());
	mouseTracker->Update(WinMouse::Get().GetState());
}