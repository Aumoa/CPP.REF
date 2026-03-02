// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/Shader.h"
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
		std::vector<byte> m_Bytecode;
		ShaderType m_Type;
		String m_EntrypointName;

	public:
		D3D12Shader(std::vector<byte> bytecode, ShaderType type, String entrypointName);
		virtual ~D3D12Shader() noexcept override;

		virtual String GetEntrypointName() const override { return m_EntrypointName; }
		virtual const byte* GetBytecode() const override { return m_Bytecode.data(); }
		virtual size_t GetBytecodeSize() const override { return m_Bytecode.size(); }
	};
}