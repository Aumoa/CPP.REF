// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
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

		virtual void UpdateData(std::span<const byte> buffer) = 0;
	};
}