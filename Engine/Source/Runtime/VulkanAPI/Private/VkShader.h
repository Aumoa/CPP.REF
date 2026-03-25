// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderCreationInfo.h"
#include "Rendering/VertexFactory.h"
#include "VkCommon.h"
#include "VkShader.gen.h"

namespace Ayla
{
	enum class ShaderType;

	ACLASS()
	class VkShader : public Shader
	{
		GENERATED_BODY()

	private:
		ShaderCreationInfo m_CreationInfo;

	public:
		VkShader(ShaderCreationInfo shaderCreationInfo);
		virtual ~VkShader() noexcept override;

		virtual VertexFactory* GetVertexFactory() const override;
		virtual bool Has(ShaderType type) const override;
		virtual const ShaderBytecode& GetBytecode(ShaderType type) const override;
	};
}
