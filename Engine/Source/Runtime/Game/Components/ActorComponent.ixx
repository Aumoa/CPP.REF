// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:ActorComponent;

import std.core;
import :GameObject;
import :TickFunction;

using namespace std;
using namespace std::chrono;

export class AActor;

/// <summary>
/// ActorComponent is the base class for components that define reusable behavior that can be added to different types of Actors.
/// </summary>
export class ActorComponent : public GameObject
{
public:
	using Super = GameObject;

	/// <summary>
	/// Represents tick function for targeted to actor component.
	/// </summary>
	class ComponentTickFunction : public TickFunction
	{
	public:
		using Super = TickFunction;

	private:
		ActorComponent* const _target = nullptr;

	public:
		/// <summary>
		/// Initialize new <see cref="ComponentTickFunction"/> instance.
		/// </summary>
		inline ComponentTickFunction(ActorComponent* target) : Super()
			, _target(target)
		{
		}

		/// <summary>
		/// Get tick function target.
		/// </summary>
		inline ActorComponent* GetTarget() const
		{
			return _target;
		}

		/// <inheritdoc/>
		virtual void ExecuteTick(duration<float> elapsedTime) override;
	};

protected:
	ComponentTickFunction PrimaryComponentTick;

private:
	uint8 _bActive : 1 = true;
	uint8 _bHasBegunPlay : 1 = false;
	AActor* _owner = nullptr;

public:
	ActorComponent(wstring_view name);

	/// <summary>
	/// Update frame tick.
	/// </summary>
	/// <param name="elapsedTime"> The elapsed time from previous frame. </param>
	/// <param name="tickFunction"> Tick function what called this. </param>
	virtual void TickComponent(duration<float> elapsedTime, ComponentTickFunction* tickFunction);

	AActor* GetOwner() const { return _owner; }
	void SetActive(bool bActive);
	inline bool IsActive() const { return _bActive; }
	inline bool HasBegunPlay() const { return _bHasBegunPlay; }
	MulticastEvent<ActorComponent, void()> Activated;
	MulticastEvent<ActorComponent, void()> Inactivated;
};