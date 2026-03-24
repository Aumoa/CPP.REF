// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "BufferUsage.h"
#include "Buffer.gen.h"

namespace Ayla
{
	ACLASS()
	class RENDERCORE_API Buffer : public Object
	{
		GENERATED_BODY()

	protected:
		Buffer();

	public:
		virtual ~Buffer() noexcept override;

		virtual size_t GetByteSize() const noexcept = 0;
		virtual size_t GetStride() const noexcept = 0;
		virtual BufferUsage GetUsage() const noexcept = 0;
		virtual void* Map() const = 0;

		size_t GetCount() const noexcept { return GetStride() > 0 ? GetByteSize() / GetStride() : 0; }
	};
}