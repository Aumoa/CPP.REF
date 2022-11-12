// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

class Misc
{
public:
	template<class T>
	static constexpr inline T& ToRef(T& inRef) noexcept
	{
		return inRef;
	}

	template<class T>
	static constexpr inline const T& ToRef(const T& inRef) noexcept
	{
		return inRef;
	}

	template<class T>
	static constexpr inline T& ToRef(T* inRef) noexcept
	{
		return *inRef;
	}

	template<class T>
	static constexpr inline const T& ToRef(const T* inRef) noexcept
	{
		return *inRef;
	}
};