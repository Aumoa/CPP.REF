// Copyright 2020 Aumoa.lib. All right reserved.

#include "Mathematics/Math.h"

#include <cmath>
#include "Mathematics/TAngleDef.h"

using namespace std;

TRadians<float> Math::Sin(float value)
{
	const TRadians<float> rad_r = sin(value);
	return rad_r;
}

TRadians<float> Math::Cos(float value)
{
	const TRadians<float> rad_r = cos(value);
	return rad_r;
}

TRadians<float> Math::Tan(float value)
{
	const TRadians<float> rad_r = tan(value);
	return rad_r;
}

TRadians<float> Math::Asin(float value)
{
	const TRadians<float> rad_r = asin(value);
	return rad_r;
}

TRadians<float> Math::Acos(float value)
{
	const TRadians<float> rad_r = acos(value);
	return rad_r;
}

TRadians<float> Math::Atan(float value)
{
	const TRadians<float> rad_r = atan(value);
	return rad_r;
}

TRadians<float> Math::Atan2(float y, float x)
{
	const TRadians<float> rad_r = atan2(y, x);
	return rad_r;
}

pair<float, float> Math::SinCos(TRadians<float> value)
{
	float quotient = (InvPI<float> * 0.5f) * value.Value;
	if (value.Value >= 0)
	{
		quotient = (float)((int32)(quotient + 0.5f));
	}
	else
	{
		quotient = (float)((int32)(quotient - 0.5f));
	}
	float y = value.Value - (2.0f * PI<float>) * quotient;

	float sign;
	if (y > HalfPI<float>)
	{
		y = PI<float> -y;
		sign = -1.0f;
	}
	else if (y < -HalfPI<float>)
	{
		y = -PI<float> -y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	float y2 = y * y;

	float sin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
	float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	float cos = sign * p;

	return { sin, cos };
}

float Math::InvSqrt(float x)
{
	constexpr int32 LogExp = 0x5F3759DF;

	int32 i = LogExp - ((int32&)x >> 1);
	float ret = (float&)i;

	ret = ret * (1.5f - (x * 0.5f * ret * ret));
	return ret;
}