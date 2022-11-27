// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/IPHostEntry.h"

IPHostEntry::IPHostEntry(std::span<const IPAddress> addresses, std::span<const String> aliases)
	: _addresses(addresses.begin(), addresses.end())
	, _aliases(aliases.begin(), aliases.end())
{
}