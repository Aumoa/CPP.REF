// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIShader;

import :RHIDeviceChild;
import :ComPtr;
import SC.Runtime.RenderCore.Internal;
import std.core;

export class RHIDevice;

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

protected:
	/// <summary>
	/// Compile vertex shader bytecode.
	/// </summary>
	virtual span<uint8 const> CompileVS() = 0;

	/// <summary>
	/// Compile pixel shader bytecode.
	/// </summary>
	virtual span<uint8 const> CompilePS() = 0;

public /*internal*/:
	ID3D12RootSignature* GetRootSignature() const { return _rs.Get(); }
	ID3D12PipelineState* GetPipelineState() const { return _ps.Get(); }
};