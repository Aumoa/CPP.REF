// Copyright 2020 Aumoa.lib. All right reserved.

#include "RHI/RHIViewport.h"

RHIViewport::RHIViewport()
{
	X = 0;
	Y = 0;
	Width = 0;
	Height = 0;
	Near = 0;
	Far = 0;
}

RHIViewport::RHIViewport(int32 x, int32 y)
{
	X = 0;
	Y = 0;
	Width = (double)x;
	Height = (double)y;
	Near = 0;
	Far = 1;
}

RHIViewport::RHIViewport(double x, double y, double width, double height, double near, double far)
{
	X = x;
	Y = y;
	Width = width;
	Height = height;
	Near = near;
	Far = far;
}

RHIViewport::~RHIViewport()
{

}