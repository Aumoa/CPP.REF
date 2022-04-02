// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <bit>

namespace libty::Sockets::inline Net
{
	class SNetUtility : virtual public SObject
	{
		GENERATED_BODY(SNetUtility);
		SNetUtility() = delete;

	public:
		template<std::integral T>
		static constexpr T HostToNetwork(T value) noexcept requires (sizeof(T) == 4)
		{
			if constexpr (std::endian::native == std::endian::little)
			{
				return ((value & 0x000000FF) << 24)
					|  ((value & 0x0000FF00) << 8)
					|  ((value & 0x00FF0000) >> 8)
					|  ((value & 0xFF000000) >> 24);
			}
			else
			{
				return value;
			}
		}
		
		template<std::integral T>
		static constexpr T HostToNetwork(T value) noexcept requires (sizeof(T) == 2)
		{
			if constexpr (std::endian::native == std::endian::little)
			{
				return ((value & 0x00FF) << 8)
					|  ((value & 0xFF00) >> 8);
			}
			else
			{
				return value;
			}
		}
	};
}