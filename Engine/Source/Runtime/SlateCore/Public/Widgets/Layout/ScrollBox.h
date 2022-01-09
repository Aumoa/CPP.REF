// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CompoundWidget.h"

class SLATECORE_API SScrollBox : public SCompoundWidget
{
	GENERATED_BODY(SScrollBox)

public:
	class SLATECORE_API SSlot : public TSlotBase<SSlot>
	{
		GENERATED_BODY(SSlot)

	public:
		SSlot() : Super()
		{
		}
	};

private:
	SPROPERTY(Slots)
	std::vector<SSlot*> Slots;

public:
	SScrollBox();
};