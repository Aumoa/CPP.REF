// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Shaders/TransparentShader/TransparentShader.h"
#include "Materials/Material.h"

#ifndef BYTE
#define BYTE uint8
#endif

//#include "TransparentShaderVS.hlsl.h"
//#include "TransparentShaderPS.hlsl.h"

STransparentShader::STransparentShader(IRHIDevice* device) : Super(device)
{
	_parameters =
	{
		// [0] Camera
		{
			L"Camera",
			ERHIShaderParameterType::ParameterCollection_CameraConstants,
			0
		},
		// [1] Color
		{
			L"Color",
			ERHIShaderParameterType::ScalarParameterConstants,
			1
		},
		// [2] Alpha
		{
			L"Alpha",
			ERHIShaderParameterType::ScalarParameterConstants,
			2
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
		},
		// [2] gAlpha
		{
			.Type = ERHIShaderParameterType::ScalarParameterConstants,
			.ScalarConstantsParameter =
			{
				.ShaderRegister = 2,
				.Num32Bits = 1
			}
		}
	};
}

std::span<uint8 const> STransparentShader::CompileVS()
{
	//return pTransparentShaderVS;
	return {};
}

std::span<uint8 const> STransparentShader::CompilePS()
{
	//return pTransparentShaderPS;
	return {};
}