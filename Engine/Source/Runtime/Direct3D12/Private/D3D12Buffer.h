// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Buffer.h"
#include "D3D12Common.h"
#include "D3D12Buffer.gen.h"

namespace Ayla
{
	class D3D12Graphics;
	enum class BufferUsage;

	ACLASS()
	class D3D12Buffer : public Buffer
	{
		GENERATED_BODY()

	private:
		ComPtr<ID3D12Resource> m_Buffer;
		BufferUsage m_Usage;

	public:
		D3D12Buffer(ComPtr<ID3D12Resource> buffer, BufferUsage usage);
		virtual ~D3D12Buffer() noexcept override;
	};
}