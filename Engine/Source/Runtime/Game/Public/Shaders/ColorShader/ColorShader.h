// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIShader.h"
#include <vector>
#include <span>

class SMaterial;

/// <summary>
/// Represents default shader bytecode.
/// </summary>
class GAME_API SColorShader : public SRHIShader
{
	GENERATED_BODY(SColorShader)

private:
	std::vector<uint8> _vscode;
	std::vector<uint8> _pscode;
	SMaterial* _material = nullptr;

public:
	SColorShader(SRHIDevice* device);

	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() const override;

	SMaterial* GetDefaultMaterial() const;

protected:
	virtual std::span<uint8 const> CompileVS() override;
	virtual std::span<uint8 const> CompilePS() override;
};