// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class ERHITextureFormat;

struct GAME_API RHIShaderBytecode
{
	using This = RHIShaderBytecode;

	const void* pShaderBytecode;
	size_t BytecodeLength;

	RHIShaderBytecode();
	RHIShaderBytecode(const void* pShaderBytecode, size_t bytecodeLength);
	RHIShaderBytecode(std::span<const uint8> shaderBytecode);
	~RHIShaderBytecode();

	vs_property_get(bool, IsValid);
	bool IsValid_get() const;
};

struct GAME_API RHIShaderDescription
{
	TRefPtr<String> ShaderName;
	RHIShaderBytecode VS;
	RHIShaderBytecode PS;
	std::vector<ERHITextureFormat> RTVFormats;
	ERHITextureFormat DSVFormat;

	RHIShaderDescription();
	~RHIShaderDescription();

	vs_property_get(bool, IsValid);
	bool IsValid_get() const;
};