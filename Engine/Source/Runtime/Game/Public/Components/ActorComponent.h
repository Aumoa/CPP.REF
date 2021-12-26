// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "Ticking/TickFunction.h"

class AActor;
class SLevel;

class GAME_API SActorComponent : public SGameObject
{
	GENERATED_BODY(SActorComponent)

private:
	class SComponentTickFunction : public TickFunction
	{
	private:
		SActorComponent* ComponentTarget = nullptr;

	public:
		SComponentTickFunction(SActorComponent* InTarget);

		virtual void ExecuteTick(float InDeltaTime) override;

		SActorComponent* GetTarget() const;
	};

protected:
	SComponentTickFunction PrimaryComponentTick;

private:
	uint8 bActive : 1 = true;
	uint8 bHasBegunPlay : 1 = false;
	uint8 bRegistered : 1 = false;

	SPROPERTY(OwnerPrivate)
	AActor* OwnerPrivate = nullptr;

public:
	SActorComponent();

	virtual void TickComponent(float InDeltaTime, SComponentTickFunction* InTickFunction);
	virtual void Tick(float InDeltaTime);

	virtual SLevel* GetLevel();
	virtual SWorld* GetWorld();

	AActor* GetOwner();

	void SetActive(bool bActive);
	void MarkOwner();
	inline bool IsActive() { return bActive; }
	inline bool HasBegunPlay() { return bHasBegunPlay; }

	DECLARE_MULTICAST_EVENT(ActivatedEvent);
	ActivatedEvent Activated;
	ActivatedEvent Inactivated;

	void RegisterComponent();
	void RegisterComponentWithWorld(SWorld* InWorld);
	void UnregisterComponent();
	bool IsRegistered();

	void DispatchBeginPlay();
	void DispatchEndPlay();

protected:
	virtual void BeginPlay();
	virtual void EndPlay();

protected:
	virtual void RegisterAllTickFunctions(SLevel* InLevel, bool bRegister);
};