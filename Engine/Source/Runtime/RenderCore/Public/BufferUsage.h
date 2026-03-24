// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "BufferUsage.gen.h"

namespace Ayla
{
	AENUM()
	enum class BufferUsage
	{
		Upload,
		VertexBuffer,
		IndexBuffer
	};
}