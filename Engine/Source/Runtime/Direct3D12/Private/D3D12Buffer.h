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
		size_t m_ByteSize;
		size_t m_Stride;

	public:
		D3D12Buffer(ComPtr<ID3D12Resource> buffer, BufferUsage usage, size_t byteSize, size_t stride);
		virtual ~D3D12Buffer() noexcept override;

		virtual size_t GetByteSize() const noexcept override { return m_ByteSize; }
		virtual size_t GetStride() const noexcept override { return m_Stride; }

		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const noexcept;
		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const noexcept;

		inline ID3D12Resource* GetResource() const noexcept { return m_Buffer.Get(); }
	};
}