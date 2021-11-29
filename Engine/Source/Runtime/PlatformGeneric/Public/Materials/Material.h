// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MaterialInterface.h"

interface IRHIShader;

class PLATFORMGENERIC_API SMaterial : public SMaterialInterface
{
	GENERATED_BODY(SMaterial)

	enum class EBaseShaderCode
	{
		UI
	};

private:
	EBaseShaderCode BaseShaderCode;
	IRHIDevice* Device = nullptr;
	IRHIShader* ShaderCode = nullptr;

public:
	SMaterial(IRHIDevice* InDevice, EBaseShaderCode InBaseShaderCode);

	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() { return {}; }

	void Compile();
	EBaseShaderCode GetBaseShaderCode();
};