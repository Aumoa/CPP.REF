// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Game
{
	template<TIsGameInstance T, class... TArgs> requires THasConstructor<T, TArgs...>
	inline int32 Application::Run(TArgs&&... args)
	{
		return RunInternal([]() -> Core::TRefPtr<GameInstance>
			{
				return NewObject<T>(std::forward<TArgs>(args)...);
			}
		);
	}
}