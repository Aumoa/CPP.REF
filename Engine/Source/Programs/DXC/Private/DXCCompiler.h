// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DXCCommon.h"

namespace Ayla
{
	class ShaderCompilationTask;

	class DXCCompiler
	{
	private:
		ComPtr<IDxcCompiler3> m_Compiler;
		ComPtr<IDxcUtils> m_Utils;

	public:
		DXCCompiler();

		Task<> CompileShaderAsync(const ShaderCompilationTask& sct, std::stop_token cancellationToken = {});

	private:
		static std::vector<String> ParseCompilerArguments(const ShaderCompilationTask& sct);
	};
}