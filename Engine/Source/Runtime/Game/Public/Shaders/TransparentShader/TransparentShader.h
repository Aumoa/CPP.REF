// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "RHI/RHIShader.h"
#include <vector>
#include <span>

class SMaterial;

class GAME_API STransparentShader : public SRHIShader
{
	GENERATED_BODY(STransparentShader)

private:
	std::vector<uint8> _vscode;
	std::vector<uint8> _pscode;
	SMaterial* _material = nullptr;

public:
	STransparentShader(SRHIDevice* device);

	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() const override;

	SMaterial* GetDefaultMaterial() const;

protected:
	virtual std::span<uint8 const> CompileVS() override;
	virtual std::span<uint8 const> CompilePS() override;
};