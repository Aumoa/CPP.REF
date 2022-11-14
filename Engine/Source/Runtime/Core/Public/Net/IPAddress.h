// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <vector>

class IPAddress
{
	std::vector<uint8> _value;

public:
	IPAddress() = default;
	IPAddress(const IPAddress&) = default;
	IPAddress(IPAddress&&) noexcept = default;

	IPAddress(uint8 v1, uint8 v2, uint8 v3, uint8 v4);
	IPAddress(uint16 v1, uint16 v2, uint16 v3, uint16 v4, uint16 v5, uint16 v6, uint16 v7, uint16 v8);

	String ToString() const;
	bool IsV4() const;

	static IPAddress Any(bool ipv6 = false);
	static IPAddress Loopback(bool ipv6 = false);

private:
	String _ipv4_to_string() const;
	String _ipv6_to_string() const;
};