// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Buffer.h"
#include "VkCommon.h"
#include "BufferUsage.h"
#include "VkBuffer.gen.h"

namespace Ayla
{
	class VkGraphics;

	ACLASS()
	class VkBuffer : public Buffer
	{
		GENERATED_BODY()

	private:
		const BufferUsage m_Usage;
		::VkBuffer m_Buffer = VK_NULL_HANDLE;
		VkDeviceMemory m_Memory = VK_NULL_HANDLE;
		VkGraphics* m_Graphics = nullptr;
		size_t m_Size = 0;
		size_t m_Stride = 0;

	public:
		::VkBuffer GetVkBuffer() const noexcept { return m_Buffer; }
		VkDeviceMemory GetVkMemory() const noexcept { return m_Memory; }
		VkDeviceAddress GetDeviceAddress() const noexcept;

		virtual size_t GetByteSize() const noexcept override { return m_Size; }
		virtual size_t GetStride() const noexcept override { return m_Stride; }
		virtual BufferUsage GetUsage() const noexcept override { return m_Usage; }
		virtual void* Map() const override;

	public:
		VkBuffer(VkGraphics* graphics, BufferUsage usage, size_t stride);
		virtual ~VkBuffer() noexcept override;

		void UpdateData(std::span<const byte> buffer);
		void AllocateUploadBuffer(size_t sizeInBytes);

	private:
		void* m_MappedPtr = nullptr;
	};
}