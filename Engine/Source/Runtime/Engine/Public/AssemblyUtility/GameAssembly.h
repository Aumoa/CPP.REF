// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance.h"

class ENGINE_API GameAssembly
{
private:
	String ModuleName;

protected:
	GameAssembly(String InModuleName);

public:
	virtual ~GameAssembly() noexcept;

	[[nodiscard]] String GetModuleName() const { return ModuleName; }

	virtual std::shared_ptr<GameInstance> CreateGameInstance() = 0;
};

#define DEFINE_GAME_ASSEMBLY_LOADER(AssemblyName) \
extern "C" \
{ \
	PLATFORM_SHARED_EXPORT GameAssembly* LoadAssemblyForCurrentModule() \
	{ \
		return new AssemblyName(); \
	} \
}
