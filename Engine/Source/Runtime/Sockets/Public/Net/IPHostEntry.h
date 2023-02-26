// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Net/IPEndPoint.h"
#include <vector>
#include <span>

class CORE_API IPHostEntry
{
	std::vector<IPAddress> _addresses;
	std::vector<String> _aliases;

public:
	IPHostEntry() noexcept = default;
	IPHostEntry(std::span<const IPAddress> addresses, std::span<const String> aliases);
	IPHostEntry(const IPHostEntry&) = default;
	IPHostEntry(IPHostEntry&&) noexcept = default;

	inline std::span<const IPAddress> GetAddresses() const noexcept { return _addresses; }
	inline std::span<const String> GetAliases() const noexcept { return _aliases; }
};