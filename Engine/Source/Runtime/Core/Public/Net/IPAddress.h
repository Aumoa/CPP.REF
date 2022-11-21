// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/String.h"
#include <array>
#include <span>

class CORE_API IPAddress
{
	friend class IPEndPoint;

	std::array<uint8, 16> _value = {};
	bool _v6 = false;

public:
	IPAddress() = default;
	IPAddress(const IPAddress&) = default;
	IPAddress(IPAddress&&) noexcept = default;

	IPAddress(uint8 v1, uint8 v2, uint8 v3, uint8 v4);
	IPAddress(uint16 v1, uint16 v2, uint16 v3, uint16 v4, uint16 v5, uint16 v6, uint16 v7, uint16 v8);
	IPAddress(String str);

	String ToString() const;
	bool IsV4() const noexcept;
	std::span<const uint8> GetBytes() const noexcept;

	static IPAddress Any(bool ipv6 = false);
	static IPAddress Loopback(bool ipv6 = false);

	static bool TryParse(String str, IPAddress* result) noexcept;
};