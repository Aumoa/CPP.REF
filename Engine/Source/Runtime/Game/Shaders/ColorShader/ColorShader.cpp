// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Core;
import SC.Runtime.Game.Shaders;
import SC.Runtime.RenderCore;
import std.core;

#define BYTE uint8

#include "ColorShaderVS.hlsl.h"
#include "ColorShaderPS.hlsl.h"

using namespace std;

ColorShader::ColorShader(RHIDevice* device) : Super(device)
{
}

span<uint8 const> ColorShader::CompileVS()
{
	return pColorShaderVS;
}

span<uint8 const> ColorShader::CompilePS()
{
	return pColorShaderPS;
}

vector<RHIShaderParameterElement> ColorShader::GetShaderParameterDeclaration() const
{
	vector<RHIShaderParameterElement> elements;

	// Camera constants.
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::ParameterCollection_CameraConstants,
		.ParameterCollection =
		{
			.ShaderRegister = 0
		}
	};

	return elements;
}