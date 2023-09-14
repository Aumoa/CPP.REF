// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHITexture2D.h"

struct NSlateRenderProxy
{
	std::shared_ptr<NRHITexture2D> Texture;
};