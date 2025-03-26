// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <cstdint>
#include <type_traits>
#include "StaticClass.h"

namespace Ayla::details
{
	template<size_t>
	constexpr bool always_false = false;

	template<size_t ByteSize, class... Types>
	struct select_arch
	{
	private:
		template<size_t CurSize, class Type, class... UTypes>
		static constexpr auto GetType()
		{
			if constexpr (CurSize == ByteSize)
			{
				return (Type*)nullptr;
			}
			else if constexpr (sizeof...(UTypes) == 0)
			{
				static_assert(always_false<CurSize>);
			}
			else
			{
				return GetType<CurSize * 2, UTypes...>();
			}
		}

	public:
		using Type = std::remove_pointer_t<decltype(GetType<2, Types...>())>;
	};

	template<size_t ByteSize, class... Types>
	using select_arch_t = typename select_arch<ByteSize, Types...>::Type;
}

namespace Ayla
{
	struct GenericPlatformTypes : public StaticClass
	{
		using uint8 = uint8_t;
		using uint16 = uint16_t;
		using uint32 = uint32_t;
		using uint64 = uint64_t;

		using int8 = int8_t;
		using int16 = int16_t;
		using int32 = int32_t;
		using int64 = int64_t;

		using byte = uint8_t;

		using ANSICHAR = char;
		using WIDECHAR = wchar_t;

		using char_t = WIDECHAR;
		using ptrdiff_t = details::select_arch_t<sizeof(void*), int16, int32, int64>;
		using size_t = details::select_arch_t<sizeof(void*), uint16, uint32, uint64>;
		using ssize_t = details::select_arch_t<sizeof(void*), int16, int32, int64>;
	};
}