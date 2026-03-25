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
		return m_CreationInfo.VertexFactory.get();
	}

	bool VkShader::Has(ShaderType type) const
	{
		switch (type)
		{
		case ShaderType::Vertex:
			return !m_CreationInfo.VertexShader.Bytecode.empty();
		case ShaderType::Pixel:
			return !m_CreationInfo.FragmentShader.Bytecode.empty();
		case ShaderType::Domain:
			return !m_CreationInfo.DomainShader.Bytecode.empty();
		case ShaderType::Hull:
			return !m_CreationInfo.HullShader.Bytecode.empty();
		case ShaderType::Geometry:
			return !m_CreationInfo.GeometryShader.Bytecode.empty();
		case ShaderType::Compute:
			return !m_CreationInfo.ComputeShader.Bytecode.empty();
		case ShaderType::RayGeneration:
			return !m_CreationInfo.RayGenerationShader.Bytecode.empty();
		case ShaderType::ClosestHit:
			return !m_CreationInfo.ClosestHitShader.Bytecode.empty();
		case ShaderType::AnyHit:
			return !m_CreationInfo.AnyHitShader.Bytecode.empty();
		case ShaderType::Miss:
			return !m_CreationInfo.MissShader.Bytecode.empty();
		default:
			return false;
		}
	}

	const ShaderBytecode& VkShader::GetBytecode(ShaderType type) const
	{
		switch (type)
		{
		case ShaderType::Vertex:
			return m_CreationInfo.VertexShader;
		case ShaderType::Pixel:
			return m_CreationInfo.FragmentShader;
		case ShaderType::Domain:
			return m_CreationInfo.DomainShader;
		case ShaderType::Hull:
			return m_CreationInfo.HullShader;
		case ShaderType::Geometry:
			return m_CreationInfo.GeometryShader;
		case ShaderType::Compute:
			return m_CreationInfo.ComputeShader;
		case ShaderType::RayGeneration:
			return m_CreationInfo.RayGenerationShader;
		case ShaderType::ClosestHit:
			return m_CreationInfo.ClosestHitShader;
		case ShaderType::AnyHit:
			return m_CreationInfo.AnyHitShader;
		case ShaderType::Miss:
			return m_CreationInfo.MissShader;
		default:
			throw InvalidOperationException(TEXT("Invalid shader type."));
		}
	}
}
