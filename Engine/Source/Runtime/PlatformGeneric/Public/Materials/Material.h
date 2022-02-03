// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MaterialInterface.h"

interface IRHIShader;

class PLATFORMGENERIC_API SMaterial : public SMaterialInterface
{
	GENERATED_BODY(SMaterial)

	SENUM(EBaseShaderCode, int32,
		UI,
	);

private:
	EBaseShaderCode BaseShaderCode;
	SPROPERTY(Device)
	IRHIDevice* Device = nullptr;
	//SPROPERTY(ShaderCode)
	//IRHIShader* ShaderCode = nullptr;

public:
	SMaterial(IRHIDevice* InDevice, EBaseShaderCode InBaseShaderCode);

	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() { return {}; }

	void Compile();
	EBaseShaderCode GetBaseShaderCode();
};