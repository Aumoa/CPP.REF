// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIEnums.h"

struct RHIStrokeStyleDesc
{
	ERHIStrokeCapStyle StartCap;
	ERHIStrokeCapStyle EndCap;
	ERHIStrokeCapStyle DashCap;
	ERHIStrokeJoin LineJoin;
	float MiterLimit;
	ERHIStrokeDashStyle DashStyle;
	float DashOffset;
};