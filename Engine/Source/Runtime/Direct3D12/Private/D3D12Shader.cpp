// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12Shader.h"

namespace Ayla
{
	D3D12Shader::D3D12Shader(ShaderCreationInfo shaderCreationInfo)
		: m_CreationInfo{ std::move(shaderCreationInfo) }
	{
	}

	D3D12Shader::~D3D12Shader() noexcept
	{
	}
}