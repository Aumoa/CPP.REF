// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/ShaderBytecode.h"
#include "Rendering/VertexFactory.h"

namespace Ayla
{
	struct ShaderCreationInfo
	{
		std::shared_ptr<VertexFactory> m_VertexFactory;
		ShaderBytecode m_VertexShader;
		ShaderBytecode m_FragmentShader;
		ShaderBytecode m_DomainShader;
		ShaderBytecode m_HullShader;
		ShaderBytecode m_GeometryShader;
		ShaderBytecode m_ComputeShader;
		ShaderBytecode m_RayGenerationShader;
		ShaderBytecode m_ClosestHitShader;
		ShaderBytecode m_AnyHitShader;
		ShaderBytecode m_MissShader;
	};
}
