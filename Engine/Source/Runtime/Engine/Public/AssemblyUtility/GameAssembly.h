// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class ENGINE_API GameAssembly
{
private:
	String ModuleName;

protected:
	GameAssembly(String InModuleName);

public:
	[[nodiscard]] String GetModuleName() const { return ModuleName; }
};

#define DEFINE_GAME_ASSEMBLY_LOADER(AssemblyName) \
extern "C" \
{ \
	PLATFORM_SHARED_EXPORT GameAssembly* LoadAssemblyForCurrentModule() \
	{ \
		return new AssemblyName(); \
	} \
}
