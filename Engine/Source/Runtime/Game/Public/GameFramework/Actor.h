// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "SubclassOf.h"
#include "Ticking/TickFunction.h"
#include "Components/SceneComponent.h"
#include <set>

DECLARE_LOG_CATEGORY(GAME_API, LogActor);

class SActorComponent;
class SSceneComponent;

/// <summary>
/// Represents actor that, spawn to world and interaction with other actors.
/// </summary>
class GAME_API AActor : public SGameObject
{
	GENERATED_BODY(AActor)

private:
	/// <summary>
	/// Represents tick function for targeted to actor.
	/// </summary>
	class SActorTickFunction : public STickFunction
	{
		GENERATED_BODY(SActorTickFunction)

	private:
		AActor* const _target = nullptr;

	public:
		inline SActorTickFunction(AActor* target) : Super()
			, _target(target)
		{
		}

		inline AActor* GetTarget() const
		{
			return _target;
		}

		/// <inheritdoc/>
		virtual void ExecuteTick(float elapsedTime) override;
	};

public:
	/// <summary>
	/// Represents primary actor tick function that call AActor::TickActor().
	/// </summary>
	SActorTickFunction PrimaryActorTick;

private:
	uint8 _bActive : 1;
	uint8 _bHasBegunPlay : 1;

public:
	/// <summary>
	/// Initialize new <see cref="AActor"/> instance.
	/// </summary>
	AActor();

	/// <summary>
	/// Update frame tick.
	/// </summary>
	/// <param name="elapsedTime"> The elapsed time from previous frame. </param>
	/// <param name="tickFunction"> Tick function what called this. </param>
	virtual void TickActor(float elapsedTime, SActorTickFunction* tickFunction);

	virtual void BeginPlay();
	virtual void EndPlay();
	virtual void Tick(float elapsedTime);

	void SetActive(bool bActive);
	inline bool IsActive() const { return _bActive; }
	inline bool HasBegunPlay() const { return _bHasBegunPlay; }
	MulticastEvent<AActor, void()> Activated;
	MulticastEvent<AActor, void()> Inactivated;

	void DestroyActor();

private:
	std::set<SActorComponent*> _Components;

public:
	void AddOwnedComponent(SActorComponent* InComponent);
	const std::set<SActorComponent*>& GetOwnedComponents();
	SActorComponent* GetComponentByClass(SubclassOf<SActorComponent> InComponentClass);

	template<std::derived_from<SActorComponent> T>
	T* GetComponentAs()
	{
		if constexpr (std::derived_from<T, SSceneComponent>)
		{
			for (auto& Component : GetSceneComponents())
			{
				if (auto* Ptr = dynamic_cast<T*>(Component))
				{
					return Ptr;
				}
			}

			return nullptr;
		}
		else  // T is derived from SActorComponent.
		{
			for (auto& Component : _Components)
			{
				if (auto* Ptr = dynamic_cast<T*>(Component))
				{
					return Ptr;
				}
			}

			return nullptr;
		}
	}

private:
	SSceneComponent* _RootComponent = nullptr;

public:
	void SetRootComponent(SSceneComponent* InRootComponent);
	SSceneComponent* GetRootComponent();
	template<std::derived_from<SSceneComponent> T>
	T* GetRootComponentAs() { return dynamic_cast<T*>(GetRootComponent()); }
	std::vector<SSceneComponent*> GetSceneComponents();

public:
	void SetActorLocation(const Vector3& location);
	Vector3 GetActorLocation() const;
	void SetActorRotation(const Quaternion& rotation);
	Quaternion GetActorRotation() const;
};