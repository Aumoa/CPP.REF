// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ShaderWorkspaces/CommonShaderWorkspace.h"
#include "CommonHLSL/CommonTypes.hlsli"

using namespace ::libty;

SCommonShaderWorkspace::SCommonShaderWorkspace(IRHIDevice* device)
	: Super()
	, _device(device)
{
	_workspace = _device->CreateShaderCodeWorkspace();
	AddHeader(L"CommonHLSL/CommonTypes.hlsli", CommonHLSL::CommonTypes);
}

void SCommonShaderWorkspace::Dispose()
{
	Dispose(true);
}

void SCommonShaderWorkspace::Compile()
{
	_workspace->Compile();

	_signature = CreateRootSignature(_workspace);
	_pipeline = CreateGraphicsPipelineState(_workspace, _signature);
}

void SCommonShaderWorkspace::Dispose(bool bDisposing)
{
	if (bDisposing)
	{
		if (_workspace)
		{
			_workspace->Dispose();
			_workspace = nullptr;
		}

		_device = nullptr;
	}
}

void SCommonShaderWorkspace::AddHeader(const std::filesystem::path& sourcePath, std::string_view code)
{
	RHIShaderCode shaderCode
	{
		.HLSLCode = std::string(code)
	};

	_workspace->AddShaderCode(sourcePath.wstring(), shaderCode);
}

void SCommonShaderWorkspace::AddVertexShader(const std::filesystem::path& sourcePath, std::string_view entryPoint, std::string_view code)
{
	RHIShaderCode shaderCode
	{
		.HLSLCode = std::string(code),
		.EntryPoint = std::string(entryPoint),
		.ShaderType = ERHIShaderType::VertexShader
	};

	_workspace->AddShaderCode(sourcePath.wstring(), shaderCode);
}

void SCommonShaderWorkspace::AddPixelShader(const std::filesystem::path& sourcePath, std::string_view entryPoint, std::string_view code)
{
	RHIShaderCode shaderCode
	{
		.HLSLCode = std::string(code),
		.EntryPoint = std::string(entryPoint),
		.ShaderType = ERHIShaderType::PixelShader
	};

	_workspace->AddShaderCode(sourcePath.wstring(), shaderCode);
}