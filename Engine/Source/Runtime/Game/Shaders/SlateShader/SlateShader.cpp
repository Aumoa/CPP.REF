// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "SlateShader.h"
#include "Materials/Material.h"

#define BYTE uint8

#include "SlateShaderVS.hlsl.h"
#include "SlateShaderPS.hlsl.h"

using namespace std;

SlateShader::SlateShader(RHIDevice* device) : Super(device)
{
	class SlateShaderMaterial : public Material
	{
	public:
		using Super = Material;

	public:
		SlateShaderMaterial(SlateShader* shader) : Super(shader)
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

	_material = CreateSubobject<SlateShaderMaterial>(this);
}

span<uint8 const> SlateShader::CompileVS()
{
	return pSlateShaderVS;
}

span<uint8 const> SlateShader::CompilePS()
{
	return pSlateShaderPS;
}

Material* SlateShader::GetDefaultMaterial() const
{
	return _material;
}

vector<RHIShaderParameterElement> SlateShader::GetShaderParameterDeclaration() const
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