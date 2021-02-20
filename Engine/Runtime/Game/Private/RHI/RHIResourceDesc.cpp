// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "RHI/RHIResourceDesc.h"

#include "RHI/RHITextureFormat.h"

RHIResourceDesc::RHIResourceDesc()
	: Format(ERHITextureFormat::Unknown)
	, Width(0)
	, Height(0)
	, DepthOrArraySize(0)
{

}

RHIResourceDesc::~RHIResourceDesc()
{

}