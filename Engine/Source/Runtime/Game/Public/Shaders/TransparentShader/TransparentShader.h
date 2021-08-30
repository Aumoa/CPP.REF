// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "RHI/RHIShader.h"
#include <vector>
#include <span>

class Material;

class GAME_API TransparentShader : public RHIShader
{
	CLASS_BODY(TransparentShader)

private:
	std::vector<uint8> _vscode;
	std::vector<uint8> _pscode;
	Material* _material = nullptr;

public:
	TransparentShader(RHIDevice* device);

	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() const override;

	Material* GetDefaultMaterial() const;

protected:
	virtual std::span<uint8 const> CompileVS() override;
	virtual std::span<uint8 const> CompilePS() override;
};