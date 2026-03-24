// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/ShaderBytecode.h"
#include "Rendering/VertexFactory.h"

namespace Ayla
{
	struct ShaderCreationInfo
	{
		std::shared_ptr<VertexFactory> VertexFactory;
		ShaderBytecode VertexShader;
		ShaderBytecode FragmentShader;
		ShaderBytecode DomainShader;
		ShaderBytecode HullShader;
		ShaderBytecode GeometryShader;
		ShaderBytecode ComputeShader;
		ShaderBytecode RayGenerationShader;
		ShaderBytecode ClosestHitShader;
		ShaderBytecode AnyHitShader;
		ShaderBytecode MissShader;
	};
}