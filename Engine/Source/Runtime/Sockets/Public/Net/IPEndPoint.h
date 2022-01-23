// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPAddress.h"

struct SOCKETS_API IPEndPoint
{
private:
	uint16 NetPort;

public:
	IPAddress IP;

	IPEndPoint();
	IPEndPoint(const IPAddress& IP, uint16 Port);
	
	void SetPort(uint16 InPort);
	uint16 GetPort() const;
	uint16 GetNetPort() const;

	std::wstring ToString() const;

	static bool TryParse(std::wstring_view IPString, IPEndPoint* RefEndPoint);
};