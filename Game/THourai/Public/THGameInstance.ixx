// Copyright 2020 Aumoa.lib. All right reserved.

#include "THAPI.h"
#include "GameInstance.h"

export module THGameInstance;
import StartupLevel;

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;

export class TH_API THGameInstance : public GameInstance
{
public:
	using Super = GameInstance;
	using This = THGameInstance;
	
private:
	TRefPtr<StartupLevel> startupLevel;

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

	Level* GetStartupLevel() override
	{
		if (!startupLevel.IsValid)
		{
			startupLevel = NewObject<StartupLevel>();
		}
		return startupLevel.Get();
	}
};