// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIShader.h"

#include "D3D12Minimal.h"

struct RHIShaderDescription;

class D3D12Shader : virtual public Object, virtual public IRHIShader
{
public:
	using Super = Object;
	using This = D3D12Shader;

private:
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12PipelineState> pipelineState;

	bool bShaderInitialized : 1;
	size_t shaderTypeHash;

public:
	D3D12Shader();
	~D3D12Shader();

	size_t GetHashCode() const override;
	size_t GetShaderTypeHashCode() const override;

	void CreateShaderPipeline(const RHIShaderDescription& inShaderDesc, ID3D12Device* device);

	vs_property_get(bool, IsInitialized);
	bool IsInitialized_get() const;

	vs_property_get(ID3D12RootSignature*, RootSignature);
	ID3D12RootSignature* RootSignature_get() const;
	vs_property_get(ID3D12PipelineState*, PipelineState);
	ID3D12PipelineState* PipelineState_get() const;

private:
	static D3D12_ROOT_PARAMETER GetRootCBVParameter(uint32 shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility);

	template<bool bFastCopy, class T>
	static inline void ApplyArray(T* outArray, std::span<T const> inArray);
};