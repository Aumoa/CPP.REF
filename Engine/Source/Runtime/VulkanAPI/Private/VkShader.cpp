// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkShader.h"
#include "Rendering/ShaderType.h"

namespace Ayla
{
	VkShader::VkShader(ShaderCreationInfo shaderCreationInfo)
		: m_CreationInfo{ std::move(shaderCreationInfo) }
	{
	}

	VkShader::~VkShader() noexcept
	{
	}

	VertexFactory* VkShader::GetVertexFactory() const
	{
		return m_CreationInfo.m_VertexFactory.get();
	}

	bool VkShader::Has(ShaderType type) const
	{
		switch (type)
		{
		case ShaderType::Vertex:
			return !m_CreationInfo.m_VertexShader.Bytecode.empty();
		case ShaderType::Pixel:
			return !m_CreationInfo.m_FragmentShader.Bytecode.empty();
		case ShaderType::Domain:
			return !m_CreationInfo.m_DomainShader.Bytecode.empty();
		case ShaderType::Hull:
			return !m_CreationInfo.m_HullShader.Bytecode.empty();
		case ShaderType::Geometry:
			return !m_CreationInfo.m_GeometryShader.Bytecode.empty();
		case ShaderType::Compute:
			return !m_CreationInfo.m_ComputeShader.Bytecode.empty();
		case ShaderType::RayGeneration:
			return !m_CreationInfo.m_RayGenerationShader.Bytecode.empty();
		case ShaderType::ClosestHit:
			return !m_CreationInfo.m_ClosestHitShader.Bytecode.empty();
		case ShaderType::AnyHit:
			return !m_CreationInfo.m_AnyHitShader.Bytecode.empty();
		case ShaderType::Miss:
			return !m_CreationInfo.m_MissShader.Bytecode.empty();
		default:
			return false;
		}
	}

	const ShaderBytecode& VkShader::GetBytecode(ShaderType type) const
	{
		switch (type)
		{
		case ShaderType::Vertex:
			return m_CreationInfo.m_VertexShader;
		case ShaderType::Pixel:
			return m_CreationInfo.m_FragmentShader;
		case ShaderType::Domain:
			return m_CreationInfo.m_DomainShader;
		case ShaderType::Hull:
			return m_CreationInfo.m_HullShader;
		case ShaderType::Geometry:
			return m_CreationInfo.m_GeometryShader;
		case ShaderType::Compute:
			return m_CreationInfo.m_ComputeShader;
		case ShaderType::RayGeneration:
			return m_CreationInfo.m_RayGenerationShader;
		case ShaderType::ClosestHit:
			return m_CreationInfo.m_ClosestHitShader;
		case ShaderType::AnyHit:
			return m_CreationInfo.m_AnyHitShader;
		case ShaderType::Miss:
			return m_CreationInfo.m_MissShader;
		default:
			throw InvalidOperationException(TEXT("Invalid shader type."));
		}
	}
}
