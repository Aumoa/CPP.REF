// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/MinimalViewInfo.h"

namespace Ayla
{
	class Buffer;

	struct SceneView
	{
		MinimalViewInfo View;
		Buffer* CameraBuffer;
		size_t CameraBufferOffset;
	};
}