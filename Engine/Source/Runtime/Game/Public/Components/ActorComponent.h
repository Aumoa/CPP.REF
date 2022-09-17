// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "Ticking/TickFunction.h"
#include "ActorComponent.generated.h"

class AActor;

SCLASS()
class GAME_API ActorComponent : public GameObject
{
	GENERATED_BODY()

private:
	class ComponentTickFunction : public TickFunction
	{
	private:
		WeakPtr<ActorComponent> ComponentTarget;

	public:
		ComponentTickFunction(ActorComponent* Target);

		virtual void ExecuteTick(const TimeSpan& InDeltaTime) override;

		inline ActorComponent* GetTarget() const noexcept { return ComponentTarget.Get(); }
	};

private:
	SPROPERTY()
	bool bActive = true;
	SPROPERTY()
	bool bHasBegunPlay = false;
	SPROPERTY()
	bool bRegistered = false;

protected:
	ComponentTickFunction PrimaryComponentTick;

public:
	ActorComponent();

	virtual void TickComponent(const TimeSpan& InDeltaTime, ComponentTickFunction* InTickFunction);

	inline bool IsActive() noexcept { return bActive; }
	inline bool HasBegunPlay() noexcept { return bHasBegunPlay; }

	AActor* GetOwner();

protected:
	virtual void Tick(const TimeSpan& InDeltaTime);
};