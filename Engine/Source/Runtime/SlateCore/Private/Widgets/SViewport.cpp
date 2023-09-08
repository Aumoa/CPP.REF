// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SViewport.h"

SViewport::SViewport()
{
}

SViewport::~SViewport() noexcept
{
}

DEFINE_SLATE_CONSTRUCTOR(SViewport, Args)
{
	PLATFORM_UNREFERENCED_PARAMETER(Args);
}

Vector2 SViewport::ComputeDesiredSize() const
{
	return Vector2::Zero();
}