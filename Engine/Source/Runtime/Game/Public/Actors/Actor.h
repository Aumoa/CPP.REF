// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "Ticking/TickFunction.h"
#include "Ticking/ITickableComponent.h"
#include "Actor.gen.h"

class Level;
class ActorComponent;
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

protected:
	ActorTickFunction PrimaryActorTick;

public:
	AActor();

	virtual void RegisterAllTickFunctions(World* InWorld);
	virtual void UnregisterAllTickFunctions(World* InWorld);

	virtual void TickComponent(const TimeSpan& InDeltaTime, ActorTickFunction* InTickFunction);

	inline bool IsActive() noexcept { return HasObjectFlags(EObjectFlags::Active); }
	inline bool HasBegunPlay() noexcept { return HasObjectFlags(EObjectFlags::BegunPlay); }
	inline bool IsRegistered() noexcept { return HasObjectFlags(EObjectFlags::Registered); }
	inline bool IsPendingKill() noexcept { return HasObjectFlags(EObjectFlags::PendingKill); }
	
	Level* GetLevel() noexcept;
	World* GetWorld() noexcept;

	void SetRootComponent(SceneComponent* InRootComponent);
	SceneComponent* GetRootComponent() noexcept;

	void GetComponents(std::list<ActorComponent*>& OutComponents, SubclassOf<ActorComponent> InComponentClass);

	void RegisterActor();
	void UnregisterActor();
	void Destroy();

protected:
	virtual void BeginPlay();
	virtual void EndPlay();
	virtual void Tick(const TimeSpan& InDeltaTime);
};