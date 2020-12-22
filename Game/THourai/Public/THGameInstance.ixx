// Copyright 2020 Aumoa.lib. All right reserved.

#include "THAPI.h"
#include "GameInstance.h"

export module THGameInstance;

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;

export class TH_API THGameInstance : public GameInstance
{
public:
	THGameInstance()
	{

	}

	~THGameInstance() override
	{

	}

	TRefPtr<String> ToString() const
	{
		return L"Touhou Hourai";
	}
};