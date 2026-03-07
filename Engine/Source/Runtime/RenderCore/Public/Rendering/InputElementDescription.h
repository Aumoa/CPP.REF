// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/InputElementSemantic.h"
#include "GraphicsFormat.h"

namespace Ayla
{
	struct InputElementDescription
	{
		InputElementSemantic SemanticName;
		int32 SemanticIndex;
		GraphicsFormat Format;
		int32 InputSlot;
		int32 AlignedByteOffset;
		int32 InstanceDataStepRate;

		InputElementDescription() = default;
		constexpr InputElementDescription(InputElementSemantic semanticName, int32 semanticIndex, GraphicsFormat format, int32 inputSlot, int32 alignedByteOffset, int32 instanceDataStepRate)
			: SemanticName(semanticName)
			, SemanticIndex(semanticIndex)
			, Format(format)
			, InputSlot(inputSlot)
			, AlignedByteOffset(alignedByteOffset)
			, InstanceDataStepRate(instanceDataStepRate)
		{
		}
	};
}