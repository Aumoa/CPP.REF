// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIShader.h"
#include <vector>
#include <span>

/// <summary>
/// Represents default shader bytecode.
/// </summary>
class ColorShader : public RHIShader
{
public:
	using Super = RHIShader;

private:
	std::vector<uint8> _vscode;
	std::vector<uint8> _pscode;

public:
	ColorShader(RHIDevice* device);

	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() const override;

protected:
	virtual std::span<uint8 const> CompileVS() override;
	virtual std::span<uint8 const> CompilePS() override;
};