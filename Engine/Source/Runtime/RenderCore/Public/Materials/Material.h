// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MaterialInterface.h"

interface IRHIShader;
class SVertexFactory;

class RENDERCORE_API SMaterial : public SMaterialInterface
{
	GENERATED_BODY(SMaterial)

private:
	IRHIDevice* _device = nullptr;
	IRHIShader* _shader = nullptr;

public:
	SMaterial(IRHIDevice* device);

	virtual void SetupCommands(IRHIDeviceContext* InContext) override { check(false); }

	/// <summary>
	/// Compile and apply shader implements.
	/// </summary>
	virtual void Compile(SVertexFactory* vertexDeclaration);

	/// <summary>
	/// Provide shader parameter declaration of this shader program.
	/// </summary>
	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() { return {}; }

	/// <summary>
	/// Compile vertex shader bytecode.
	/// </summary>
	virtual std::span<uint8 const> CompileVS() = 0;

	/// <summary>
	/// Compile pixel shader bytecode.
	/// </summary>
	virtual std::span<uint8 const> CompilePS() = 0;

	IRHIShader* GetShader() { return _shader; }
};