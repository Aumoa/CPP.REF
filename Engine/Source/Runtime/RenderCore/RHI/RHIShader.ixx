// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIShader;

import :RHIDeviceChild;
import :ComPtr;
import SC.Runtime.RenderCore.Internal;
import std.core;
import :RHIStructures;

export class RHIDevice;
export class RHIResource;

using namespace std;

/// <summary>
/// Represents shader implements.
/// </summary>
export class RHIShader : public RHIDeviceChild
{
public:
	using Super = RHIDeviceChild;

private:
	ComPtr<ID3D12RootSignature> _rs;
	ComPtr<ID3D12PipelineState> _ps;

public:
	RHIShader(RHIDevice* device);
	~RHIShader();

	/// <summary>
	/// Compile and apply shader implements.
	/// </summary>
	virtual void Compile();

	/// <summary>
	/// Create vertex buffer using vertex declaration of this shader program.
	/// </summary>
	virtual RHIResource* CreateVertexBuffer(const RHIVertex* vertices, size_t count) const;

	/// <summary>
	/// Create index buffer.
	/// </summary>
	virtual RHIResource* CreateIndexBuffer(const uint32* indices, size_t count) const;

	/// <summary>
	/// Get vertex stride.
	/// </summary>
	virtual uint32 GetVertexStride() const = 0;

protected:
	/// <summary>
	/// Compile vertex shader bytecode.
	/// </summary>
	virtual span<uint8 const> CompileVS() = 0;

	/// <summary>
	/// Compile pixel shader bytecode.
	/// </summary>
	virtual span<uint8 const> CompilePS() = 0;

	/// <summary>
	/// Provide vertex declaration of this shader program.
	/// </summary>
	virtual vector<RHIVertexElement> GetVertexDeclaration() const { return {}; }

	/// <summary>
	/// Provide shader parameter declaration of this shader program.
	/// </summary>
	virtual vector<RHIShaderParameterElement> GetShaderParameterDeclaration() const { return {}; }

public /*internal*/:
	ID3D12RootSignature* GetRootSignature() const { return _rs.Get(); }
	ID3D12PipelineState* GetPipelineState() const { return _ps.Get(); }
};