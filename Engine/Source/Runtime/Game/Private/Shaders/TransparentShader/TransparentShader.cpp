// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Shaders/TransparentShader/TransparentShader.h"
#include "Materials/Material.h"

#ifndef BYTE
#define BYTE uint8
#endif

#include "TransparentShaderVS.hlsl.h"
#include "TransparentShaderPS.hlsl.h"

TransparentShader::TransparentShader(RHIDevice* device) : Super(device)
{
	class TransparentShaderMaterial : public Material
	{
	public:
		using Super = Material;

	public:
		TransparentShaderMaterial(TransparentShader* shader) : Super(shader)
		{
			_BlendMode = EMaterialBlendMode::Transparent;
		}

		virtual int32 GetRootParameterMappingIndex(std::wstring_view parameterName) const override
		{
			if (parameterName == L"Color")
			{
				return 1;
			}
			else if (parameterName == L"Alpha")
			{
				return 2;
			}
			else
			{
				return Super::GetRootParameterMappingIndex(parameterName);
			}
		}
	};

	_material = CreateSubobject<TransparentShaderMaterial>(this);
}

std::span<uint8 const> TransparentShader::CompileVS()
{
	return pTransparentShaderVS;
}

std::span<uint8 const> TransparentShader::CompilePS()
{
	return pTransparentShaderPS;
}

Material* TransparentShader::GetDefaultMaterial() const
{
	return _material;
}

std::vector<RHIShaderParameterElement> TransparentShader::GetShaderParameterDeclaration() const
{
	std::vector<RHIShaderParameterElement> elements;

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

	// [2] gAlpha
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::ScalarParameterConstants,
		.ScalarConstantsParameter =
		{
			.ShaderRegister = 2,
			.Num32Bits = 1
		}
	};

	return elements;
}