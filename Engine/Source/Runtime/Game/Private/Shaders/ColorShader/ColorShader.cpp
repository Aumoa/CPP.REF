// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Shaders/ColorShader/ColorShader.h"
#include "Materials/Material.h"

#define BYTE uint8

#include "ColorShaderVS.hlsl.h"
#include "ColorShaderPS.hlsl.h"

using namespace std;

ColorShader::ColorShader(RHIDevice* device) : Super(device)
{
	class ColorShaderMaterial : public Material
	{
	public:
		using Super = Material;

	public:
		ColorShaderMaterial(ColorShader* shader) : Super(shader)
		{
		}

		virtual int32 GetRootParameterMappingIndex(std::wstring_view parameterName) const override
		{
			if (parameterName == L"Color")
			{
				return 1;
			}
			else
			{
				return Super::GetRootParameterMappingIndex(parameterName);
			}
		}
	};

	_material = CreateSubobject<ColorShaderMaterial>(this);
}

span<uint8 const> ColorShader::CompileVS()
{
	return pColorShaderVS;
}

span<uint8 const> ColorShader::CompilePS()
{
	return pColorShaderPS;
}

Material* ColorShader::GetDefaultMaterial() const
{
	return _material;
}

vector<RHIShaderParameterElement> ColorShader::GetShaderParameterDeclaration() const
{
	vector<RHIShaderParameterElement> elements;

	// [0] Camera constants.
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::ParameterCollection_CameraConstants,
		.ParameterCollection =
		{
			.ShaderRegister = 0
		}
	};

	// [1] gColor
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::ScalarParameterConstants,
		.ScalarConstantsParameter =
		{
			.ShaderRegister = 1,
			.Num32Bits = 3
		}
	};

	return elements;
}