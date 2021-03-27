// Copyright 2020-2021 Aumoa.lib. All right reserved.

#ifndef __PRIMITIVEPACKING_HLSLI__
#define __PRIMITIVEPACKING_HLSLI__

float ConvUInt8ToSingle(uint value)
{
	uint _8_bit = value & 0xFF;
	return (float)_8_bit / 255.0f;
}

uint ConvSingleToUInt8(float value)
{
	float _8_bit = round(saturate(value) * 255.0f);
	return (uint)_8_bit;
}

uint PackUInt8x2(uint first, uint second)
{
	uint first_8 = first & 0xFF;
	uint second_8 = second & 0xFF;
	return first_8 << 8 | second_8;
}

uint2 UnpackUIntToUInt8x2(uint value)
{
	uint first_8 = (value >> 8) & 0xFF;
	uint second_8 = value & 0xFF;
	return uint2(first_8, second_8);
}

float3 ConvUInt16ToSnorm16(uint3 value)
{
	float3 r = (float3)value / 50000.0f;
	return r * 2.0f - 1.0f;
}

uint3 ConvSnorm16ToUInt16(float3 value)
{
	float3 r = (value + 1.0f) * 0.5f;
	return (uint3)(r * 50000.0f);
}

#endif