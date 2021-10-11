// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "Ticking/TickFunction.h"

class AActor;

/// <summary>
/// ActorComponent is the base class for components that define reusable behavior that can be added to different types of Actors.
/// </summary>
class GAME_API SActorComponent : public SGameObject
{
	GENERATED_BODY(SActorComponent)

private:
	/// <summary>
	/// Represents tick function for targeted to actor component.
	/// </summary>
	class SComponentTickFunction : public STickFunction
	{
		GENERATED_BODY(SComponentTickFunction)

	private:
		SActorComponent* const _target = nullptr;

	public:
		/// <summary>
		/// Initialize new <see cref="ComponentTickFunction"/> instance.
		/// </summary>
		inline SComponentTickFunction(SActorComponent* target) : Super()
			, _target(target)
		{
		}

		/// <summary>
		/// Get tick function target.
		/// </summary>
		inline SActorComponent* GetTarget() const
		{
			return _target;
		}

		/// <inheritdoc/>
		virtual void ExecuteTick(float elapsedTime) override;
	};

protected:
	SComponentTickFunction PrimaryComponentTick;

private:
	uint8 _bActive : 1 = true;
	uint8 _bHasBegunPlay : 1 = false;
	uint8 _bRegistered : 1 = false;
	AActor* _owner = nullptr;

public:
	SActorComponent();

	/// <summary>
	/// Update frame tick.
	/// </summary>
	/// <param name="elapsedTime"> The elapsed time from previous frame. </param>
	/// <param name="tickFunction"> Tick function what called this. </param>
	virtual void TickComponent(float elapsedTime, SComponentTickFunction* tickFunction);

	virtual void BeginPlay();
	virtual void EndPlay();
	virtual void Tick(float elapsedTime);

	AActor* GetOwner() { return _owner; }
	void SetActive(bool bActive);
	inline bool IsActive() { return _bActive; }
	inline bool HasBegunPlay() { return _bHasBegunPlay; }

	DECLARE_MULTICAST_EVENT(ActivatedEvent);
	ActivatedEvent Activated;
	ActivatedEvent Inactivated;

	void RegisterComponent();
	void RegisterComponentWithWorld(SWorld* world);
	void UnregisterComponent();
	bool IsRegistered();

	void SetOwnerPrivate(AActor* owner) { _owner = owner; }
};