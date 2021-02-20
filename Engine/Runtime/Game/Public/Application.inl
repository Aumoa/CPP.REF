// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<TIsGameInstance T, class... TArgs> requires THasConstructor<T, TArgs...>
inline int32 Application::Run(TArgs&&... args)
{
	return RunInternal([]() -> TRefPtr<GameInstance>
		{
			return NewObject<T>(std::forward<TArgs>(args)...);
		}
	);
}