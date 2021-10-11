// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include <set>
#include "Ticking/TickFunction.h"
#include "Components/SceneComponent.h"

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

protected:
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

	void RegisterActorWithWorld(SWorld* world);
	void DestroyActor();

private:
	std::set<SActorComponent*> _components;

public:
	void AddOwnedComponent(SActorComponent* component);
	std::set<SActorComponent*> GetOwnedComponents() const;
	template<std::derived_from<SActorComponent> T>
	T* GetComponentAs() const
	{
		// Find component from actor components.
		for (auto& component : _components)
		{
			if (auto* ptr = dynamic_cast<T*>(component); ptr != nullptr)
			{
				return ptr;
			}
		}

		if constexpr (std::derived_from<T, SSceneComponent>)
		{
			// Else, find component from scene components.
			T* item = nullptr;
			ForEachSceneComponents<T>([&item](T* component)
				{
					item = component;
					return false;
				});

			return item;
		}

		return nullptr;
	}

private:
	SSceneComponent* _rootComponent = nullptr;

public:
	void SetRootComponent(SSceneComponent* scene);
	SSceneComponent* GetRootComponent() const;
	template<std::derived_from<SSceneComponent> T>
	T* GetRootComponentAs() const
	{
		return dynamic_cast<T*>(GetRootComponent());
	}

	template<std::derived_from<SSceneComponent> T>
	void ForEachSceneComponents(std::function<bool(T*)> body) const
	{
		if (_rootComponent == nullptr)
		{
			return;
		}

		_rootComponent->ForEachSceneComponents<T>(body);
	}

public:
	void SetActorLocation(const Vector3& location);
	Vector3 GetActorLocation() const;
	void SetActorRotation(const Quaternion& rotation);
	Quaternion GetActorRotation() const;
};