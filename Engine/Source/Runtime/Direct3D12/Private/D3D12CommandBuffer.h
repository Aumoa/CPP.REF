// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommandBuffer.h"
#include "D3D12Common.h"
#include "D3D12CommandBuffer.gen.h"

namespace Ayla
{
	class D3D12Graphics;

	ACLASS()
	class D3D12CommandBuffer : public CommandBuffer
	{
		GENERATED_BODY()

	private:
		ID3D12CommandQueue* m_Queue;
		ComPtr<ID3D12CommandAllocator> m_CommandPool;
		ComPtr<ID3D12GraphicsCommandList> m_CommandBuffer;

	public:
		D3D12CommandBuffer(D3D12Graphics* graphics);
		virtual ~D3D12CommandBuffer() noexcept override;

		virtual void Dispose() noexcept override;

		virtual void BeginCommands_Implementation() override;
		virtual void EndCommands_Implementation() override;

		inline ID3D12GraphicsCommandList* GetCommandBuffer() const noexcept { return m_CommandBuffer.Get(); }
	};
}