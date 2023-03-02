// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Guid.h"
#include "CoreTypes/String.h"
#include <vector>
#include <random>
#include <string>

String Guid::ToString() const
{
	const _InternalPad& iPad = *reinterpret_cast<const _InternalPad*>(_data4.data());
	return String::Format(TEXT("{:0>8X}-{:0>4X}-{:0>4X}-{:0>4X}-{:0>12X}"), _data1, _data2, _data3, iPad._data4, iPad._data5);
}

bool Guid::TryParse(String formattedString, Guid& outResult)
{
	String trimmedString = formattedString.Trim(std::array<wchar_t, 5>{ '{', '}', ' ', '\t', '\n' });
	std::vector<String> splits = trimmedString.Split(TEXT("-"), EStringSplitOptions::RemoveEmptyEntries | EStringSplitOptions::TrimEntries);
	if (splits.size() != 5)
	{
		return false;
	}

	try
	{
		uint32 data1 = std::stoul((std::wstring)splits[0], nullptr, 16);
		uint16 data2 = (uint16)std::stoul((std::wstring)splits[1], nullptr, 16);
		uint16 data3 = (uint16)std::stoul((std::wstring)splits[2], nullptr, 16);

		_InternalPad iPad
		{
			._data4 = (uint16)std::stoul((std::wstring)splits[3], nullptr, 16),
			._data5 = (uint64)std::stoull((std::wstring)splits[4], nullptr, 16)
		};

		outResult._data1 = data1;
		outResult._data2 = data2;
		outResult._data3 = data3;
		memcpy(outResult._data4.data(), &iPad, sizeof(_InternalPad));
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

Guid Guid::NewGuid()
{
	static thread_local std::random_device rd;
	static thread_local std::mt19937_64 gen(rd());

	int64 gen_1 = gen();
	int64 gen_2 = gen();

	_InternalUnion uv
	{
		._uniform1 = (int64)gen(),
		._uniform2 = (int64)gen()
	};

	return reinterpret_cast<Guid&>(uv);
}