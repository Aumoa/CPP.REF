// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Net/Packet.h"
#include "Net/IPAddress.h"

template<class key_t, class value_t>
inline Packet& operator <<(Packet& os, const std::map<key_t, value_t>& p)
{
	os << (int32)p.size();
	for (auto& pair : p)
	{
		os << pair.first << pair.second;
	}
	return os;
}

template<class key_t, class value_t>
inline Packet& operator >>(Packet& is, std::map<key_t, value_t>& p)
{
	int32 cnt = is.Read<int32>();
	for (int32 i = 0; i < cnt; ++i)
	{
		key_t k;
		value_t v;
		is >> k >> v;
		p.emplace(std::move(k), std::move(v));
	}
	return is;
}

inline Packet& operator <<(Packet& is, const IPAddress& p)
{
	bool isV4 = p.IsV4();
	if (isV4)
	{
		auto bytes = p.GetBytes();
		is << isV4 << bytes[0] << bytes[1] << bytes[2] << bytes[3];
	}
	else
	{
		const uint16* ptr = reinterpret_cast<const uint16*>(p.GetBytes().data());
		is << isV4 << ptr[0] << ptr[1] << ptr[2] << ptr[3] << ptr[4] << ptr[5] << ptr[6] << ptr[7];
	}
	return is;
}

inline Packet& operator >>(Packet& os, IPAddress& p)
{
	bool isV4;
	os >> isV4;
	if (isV4)
	{
		uint8 values[4];
		os >> values[0] >> values[1] >> values[2] >> values[3];
		p = IPAddress(values[0], values[1], values[2], values[3]);
	}
	else
	{
		uint16 values[8];
		os >> values[0] >> values[1] >> values[2] >> values[3]
			>> values[4] >> values[5] >> values[6] >> values[7];

		p = IPAddress(values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7]);
	}
	return os;
}