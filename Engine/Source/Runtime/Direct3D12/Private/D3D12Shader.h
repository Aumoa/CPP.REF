// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderCreationInfo.h"
#include "Rendering/VertexFactory.h"
#include "D3D12Common.h"
#include "D3D12Shader.gen.h"

namespace Ayla
{
	enum class ShaderType;

	ACLASS()
	class D3D12Shader : public Shader
	{
		GENERATED_BODY()

	private:
		ShaderCreationInfo m_CreationInfo;

	public:
		D3D12Shader(ShaderCreationInfo shaderCreationInfo);
		virtual ~D3D12Shader() noexcept override;

		virtual VertexFactory* GetVertexFactory() const override;
		virtual bool Has(ShaderType type) const override;
		virtual const ShaderBytecode& GetBytecode(ShaderType type) const override;
	};
}