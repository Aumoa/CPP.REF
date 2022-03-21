// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "IDisposable.h"
#include "RHI/RHIInterfaces.h"
#include <filesystem>

class RENDERCORE_API SCommonShaderWorkspace : implements SObject, implements IDisposable
{
	GENERATED_BODY(SCommonShaderWorkspace)

private:
	SPROPERTY(_device)
	IRHIDevice* _device = nullptr;
	SPROPERTY(_workspace)
	IRHIShaderCodeWorkspace* _workspace = nullptr;

	SPROPERTY(_signature)
	IRHIRootSignature* _signature = nullptr;
	SPROPERTY(_pipeline)
	IRHIPipelineState* _pipeline = nullptr;

public:
	SCommonShaderWorkspace(IRHIDevice* device);

	virtual void Dispose();
	virtual void Compile();

protected:
	virtual void Dispose(bool bDisposing) override;

	virtual IRHIRootSignature* CreateRootSignature(IRHIShaderCodeWorkspace* workspace) = 0;
	virtual IRHIPipelineState* CreateGraphicsPipelineState(IRHIShaderCodeWorkspace* workspace, IRHIRootSignature* signature) = 0;

protected:
	void AddHeader(const std::filesystem::path& sourcePath, std::string_view code);
	void AddVertexShader(const std::filesystem::path& sourcePath, std::string_view entryPoint, std::string_view code);
	void AddPixelShader(const std::filesystem::path& sourcePath, std::string_view entryPoint, std::string_view code);
};