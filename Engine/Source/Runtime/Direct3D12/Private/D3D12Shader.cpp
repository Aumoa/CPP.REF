// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12Shader.h"

namespace Ayla
{
	D3D12Shader::D3D12Shader(std::vector<byte> bytecode, ShaderType type, String entrypointName)
		: m_Bytecode(std::move(bytecode))
		, m_Type(type)
		, m_EntrypointName(std::move(entrypointName))
	{
	}

	D3D12Shader::~D3D12Shader() noexcept
	{
	}
}