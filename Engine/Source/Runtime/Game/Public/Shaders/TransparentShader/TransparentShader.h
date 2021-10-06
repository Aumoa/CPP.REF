// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Materials/Material.h"
#include <vector>
#include <span>

class GAME_API STransparentShader : public SMaterial
{
	GENERATED_BODY(STransparentShader)

private:
	std::vector<uint8> _vscode;
	std::vector<uint8> _pscode;

	std::vector<RHIMaterialParameterInfo> _parameters;
	std::vector<RHIShaderParameterElement> _elements;

public:
	STransparentShader(IRHIDevice* device);

	virtual std::vector<RHIMaterialParameterInfo> GetParametersInfo() override { return _parameters; }

	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() override { return _elements; }
	virtual std::span<uint8 const> CompileVS() override;
	virtual std::span<uint8 const> CompilePS() override;
};