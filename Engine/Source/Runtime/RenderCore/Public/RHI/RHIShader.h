// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"
#include <vector>
#include <span>
#include "ComPtr.h"
#include "RHIStructures.h"

struct ID3D12RootSignature;
struct ID3D12PipelineState;
class RHIVertexFactory;

/// <summary>
/// Represents shader implements.
/// </summary>
class RENDERCORE_API RHIShader : public RHIDeviceChild
{
	GENERATED_BODY(RHIShader)

private:
	RHIVertexFactory* _vfactory = nullptr;
	ComPtr<ID3D12RootSignature> _rs;
	ComPtr<ID3D12PipelineState> _ps;

public:
	RHIShader(RHIDevice* device);
	~RHIShader();

	/// <summary>
	/// Compile and apply shader implements.
	/// </summary>
	virtual void Compile(RHIVertexFactory* vertexDeclaration);

	/// <summary>
	/// Provide shader parameter declaration of this shader program.
	/// </summary>
	virtual std::vector<RHIShaderParameterElement> GetShaderParameterDeclaration() const { return {}; }

protected:
	/// <summary>
	/// Compile vertex shader bytecode.
	/// </summary>
	virtual std::span<uint8 const> CompileVS() = 0;

	/// <summary>
	/// Compile pixel shader bytecode.
	/// </summary>
	virtual std::span<uint8 const> CompilePS() = 0;

public /*internal*/:
	ID3D12RootSignature* GetRootSignature() const { return _rs.Get(); }
	ID3D12PipelineState* GetPipelineState() const { return _ps.Get(); }
};