// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:GameConcepts;

import :AActor;

export
{
	/// <summary>
	/// Represents the type is subclass of AActor.
	/// </summary>
	template<class T>
	concept IsActorClass = requires(T* instance, AActor* actor)
	{
		{ actor = instance };
	};
}