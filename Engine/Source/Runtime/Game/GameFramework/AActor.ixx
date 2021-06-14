// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:AActor;

import std.core;
import SC.Runtime.Core;
import :GameObject;
import :TickFunction;
import :SceneComponent;

using namespace std;
using namespace std::chrono;

/// <summary>
/// Represents actor that, spawn to world and interaction with other actors.
/// </summary>
export class AActor : public GameObject
{
public:
	using Super = GameObject;

	/// <summary>
	/// Represents tick function for targeted to actor.
	/// </summary>
	class ActorTickFunction : public TickFunction
	{
	public:
		using Super = TickFunction;

	private:
		AActor* const _target = nullptr;

	public:
		/// <summary>
		/// Initialize new <see cref="ActorTickFunction"/> instance.
		/// </summary>
		inline ActorTickFunction(AActor* target) : Super()
			, _target(target)
		{
		}

		/// <summary>
		/// Get tick function target.
		/// </summary>
		inline AActor* GetTarget() const
		{
			return _target;
		}

		/// <inheritdoc/>
		virtual void ExecuteTick(duration<float> elapsedTime) override;
	};

protected:
	/// <summary>
	/// Represents primary actor tick function that call AActor::TickActor().
	/// </summary>
	ActorTickFunction PrimaryActorTick;

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
	virtual void TickActor(duration<float> elapsedTime, ActorTickFunction* tickFunction);

	virtual void BeginPlay();
	virtual void EndPlay();

	void SetActive(bool bActive);
	inline bool IsActive() const { return _bActive; }
	inline bool HasBegunPlay() const { return _bHasBegunPlay; }
	MulticastEvent<AActor, void()> Activated;
	MulticastEvent<AActor, void()> Inactivated;

	void RegisterActorWithWorld(World* world);

private:
	set<ActorComponent*> _components;

public:
	set<ActorComponent*> GetOwnedComponents() const;
	template<derived_from<ActorComponent> T>
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

		if constexpr (derived_from<T, SceneComponent>)
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
	SceneComponent* _rootComponent = nullptr;

public:
	void SetRootComponent(SceneComponent* scene);
	SceneComponent* GetRootComponent() const;
	template<derived_from<SceneComponent> T>
	T* GetRootComponentAs() const
	{
		return dynamic_cast<T*>(GetRootComponent());
	}

	template<derived_from<SceneComponent> T>
	void ForEachSceneComponents(function<bool(T*)> body) const
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