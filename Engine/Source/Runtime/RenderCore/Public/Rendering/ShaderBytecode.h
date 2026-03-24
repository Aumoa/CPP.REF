// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	struct ShaderBytecode
	{
		std::vector<byte> Bytecode;
		String EntrypointName;
	};
}