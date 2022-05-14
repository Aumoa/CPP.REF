// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/String.h"
#include <array>

namespace libty::inline Core
{
	class CORE_API Guid
	{
		uint32 _data1;
		uint16 _data2;
		uint16 _data3;
		std::array<uint8, 8> _data4;

		struct _InternalPad
		{
			uint64 _data4 : 16;
			uint64 _data5 : 48;
		};

		struct _InternalUnion
		{
			int64 _uniform1;
			int64 _uniform2;
		};

	public:
		constexpr Guid()
			: _data1(0)
			, _data2(0)
			, _data3(0)
			, _data4{}
		{
		}

		constexpr Guid(uint32 data1, uint16 data2, uint16 data3, uint8 data40, uint8 data41, uint8 data42, uint8 data43, uint8 data44, uint8 data45, uint8 data46, uint8 data47)
			: _data1(data1)
			, _data2(data2)
			, _data3(data3)
			, _data4{ data40, data41, data42, data43, data44, data45, data46, data47 }
		{
		}

		String ToString() const;

		static bool TryParse(String formattedString, Guid& outResult);
		static Guid NewGuid();
	};
}