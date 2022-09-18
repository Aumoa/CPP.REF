// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "Ticking/TickFunction.h"
#include "Ticking/ITickableComponent.h"
#include "Actor.generated.h"

class SceneComponent;

SCLASS()
class GAME_API AActor : public GameObject, virtual public ITickableComponent
{
	GENERATED_BODY()

private:
	class ActorTickFunction : public TickFunction
	{
	private:
		WeakPtr<AActor> Target;

	public:
		ActorTickFunction(AActor* InTarget);

		AActor* GetTarget() const noexcept;
		virtual void ExecuteTick(const TimeSpan& InDeltaTime) override;
	};

private:
	SPROPERTY()
	SceneComponent* RootComponent = nullptr;
	SPROPERTY()
	bool bActive = true;
	SPROPERTY()
	bool bHasBegunPlay = false;

protected:
	ActorTickFunction PrimaryActorTick;

public:
	AActor();

	virtual void RegisterAllTickFunctions(World* InWorld);
	virtual void UnregisterAllTickFunctions(World* InWorld);

	virtual void TickComponent(const TimeSpan& InDeltaTime, ActorTickFunction* InTickFunction);

	inline bool IsActive() noexcept { return bActive; }
	inline bool HasBegunPlay() noexcept { return bHasBegunPlay; }

	void SetRootComponent(SceneComponent* InRootComponent);
	SceneComponent* GetRootComponent() noexcept;

	void DispatchBeginPlay(World* InWorld);
	void DispatchEndPlay(World* InWorld);

protected:
	virtual void Tick(const TimeSpan& InDeltaTime);

};