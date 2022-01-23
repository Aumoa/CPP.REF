// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct SOCKETS_API IPAddress
{
	union
	{
		uint32 Address;
		uint8 ByteAddress[4];
	};

	std::wstring ToString() const;

	static IPAddress Any();
	static IPAddress Loopback();
	static bool TryParse(std::wstring_view IPString, IPAddress* RefAddress);
	static bool TryParseHostname(std::wstring_view Hostname, std::vector<IPAddress>* RefAddresses);
};