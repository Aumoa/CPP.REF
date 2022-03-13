// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/String.h"
#include <array>
#include <vector>
#include <random>
#include <string>

class CORE_API Guid
{
	uint32 _data1;
	uint16 _data2;
	uint16 _data3;
	uint8 _data4[8];

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

	std::wstring ToString() const
	{
		const _InternalPad& iPad = *reinterpret_cast<const _InternalPad*>(_data4);
		return std::format(L"{:0>8X}-{:0>4X}-{:0>4X}-{:0>4X}-{:0>12X}", _data1, _data2, _data3, iPad._data4, iPad._data5);
	}

	static bool TryParse(std::wstring_view formattedString, Guid& outResult)
	{
		std::wstring trimmedString = String::Trim(formattedString, std::array<wchar_t, 5>{ L'{', L'}', L' ', L'\t', L'\n' });
		static_assert(IString<wchar_t[10], wchar_t>, "!IString");
		std::vector<std::wstring> splits = String::Split(trimmedString, L"-", true, true);
		if (splits.size() != 5)
		{
			return false;
		}

		try
		{
			uint32 data1 = std::stoul(splits[0], nullptr, 16);
			uint16 data2 = (uint16)std::stoul(splits[1], nullptr, 16);
			uint16 data3 = (uint16)std::stoul(splits[2], nullptr, 16);

			_InternalPad iPad;
			iPad._data4 = (uint16)std::stoul(splits[3], nullptr, 16);
			iPad._data5 = (uint64)std::stoull(splits[4], nullptr, 16);

			outResult._data1 = data1;
			outResult._data2 = data2;
			outResult._data3 = data3;
			memcpy(outResult._data4, &iPad, sizeof(_InternalPad));
			return true;
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	static Guid NewGuid()
	{
		static thread_local std::random_device rd;
		static thread_local std::mt19937_64 gen(rd());

		int64 gen_1 = gen();
		int64 gen_2 = gen();

		_InternalUnion uv;
		uv._uniform1 = gen();
		uv._uniform2 = gen();

		return reinterpret_cast<Guid&>(uv);
	}
};