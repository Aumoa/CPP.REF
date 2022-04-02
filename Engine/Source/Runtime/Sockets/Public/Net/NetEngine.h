// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Sockets/Socket.h"

class SOCKETS_API NetEngine
{
	NetEngine() = default;

private:
	//Socket _passiveSession;

public:
	static NetEngine* Get();

	void CreatePassiveSession();
};