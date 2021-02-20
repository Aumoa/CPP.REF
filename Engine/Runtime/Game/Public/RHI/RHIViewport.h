// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct GAME_API RHIViewport
{
	double X;
	double Y;
	double Width;
	double Height;
	double Near;
	double Far;

	RHIViewport();
	RHIViewport(int32 x, int32 y);
	RHIViewport(double x, double y, double width, double height, double near, double far);
	~RHIViewport();
};