// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "RHI/RHIShader.h"
#include <vector>
#include <span>

class Material;

class SlateShader : public RHIShader
{
public:
	using Super = RHIShader;

private:
	std::vector<uint8> _vscode;
	std::vector<uint8> _pscode;
	Material* _material = nullptr;

public:
	SlateShader(RHIDevice* device);

	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() const override;

	Material* GetDefaultMaterial() const;

protected:
	virtual std::span<uint8 const> CompileVS() override;
	virtual std::span<uint8 const> CompilePS() override;
};