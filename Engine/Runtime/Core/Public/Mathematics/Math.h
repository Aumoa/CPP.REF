// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Object.h"

namespace SC::Runtime::Core::Mathematics
{
	template<class T>
	concept THasLerp = requires()
	{
		{ T::Lerp(T(), T(), (double)0) };
	};

	template<class T>
	constexpr bool VHasLerp = false;
	template<THasLerp T>
	constexpr bool VHasLerp<T> = true;

	class CORE_API Math abstract final : virtual public Object
	{
	public:
		using Super = Object;
		using This = Math;

		template<class T>
		inline static T Lerp(const T& left, const T& right, double t);
	};
}