// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:AActor;

import std.core;
import SC.Runtime.Core;
import :GameObject;
import :TickFunction;

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

public:
	/// <summary>
	/// Initialize new <see cref="AActor"/> instance.
	/// </summary>
	/// <param name="name"> The actor name. </param>
	AActor(wstring_view name);

	/// <summary>
	/// Update frame tick.
	/// </summary>
	/// <param name="elapsedTime"> The elapsed time from previous frame. </param>
	/// <param name="tickFunction"> Tick function what called this. </param>
	virtual void TickActor(duration<float> elapsedTime, ActorTickFunction* tickFunction);
};