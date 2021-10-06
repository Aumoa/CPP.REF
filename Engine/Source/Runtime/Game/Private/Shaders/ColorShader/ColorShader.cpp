// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Shaders/ColorShader/ColorShader.h"
#include "Materials/Material.h"

#ifndef BYTE
#define BYTE uint8
#endif

#include "ColorShaderVS.hlsl.h"
#include "ColorShaderPS.hlsl.h"

SColorShader::SColorShader(IRHIDevice* device) : Super(device)
{
	_parameters =
	{
		// [0] Camera
		{
			L"Camera",
			ERHIShaderParameterType::ParameterCollection_CameraConstants,
			0,
		},
		// [1] Camera
		{
			L"Color",
			ERHIShaderParameterType::ScalarParameterConstants,
			1,
		},
	};

	_elements =
	{
		// [0] Camera constants.
		{
			.Type = ERHIShaderParameterType::ParameterCollection_CameraConstants,
			.ParameterCollection =
			{
				.ShaderRegister = 0
			}
		},
		// [1] gColor
		{
			.Type = ERHIShaderParameterType::ScalarParameterConstants,
			.ScalarConstantsParameter =
			{
				.ShaderRegister = 1,
				.Num32Bits = 3
			}
		}
	};
}

std::span<uint8 const> SColorShader::CompileVS()
{
	return pColorShaderVS;
}

std::span<uint8 const> SColorShader::CompilePS()
{
	return pColorShaderPS;
}