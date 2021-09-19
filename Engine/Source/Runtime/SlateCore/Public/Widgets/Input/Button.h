// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/Border.h"

class SLATECORE_API SButton : public SBorder
{
	GENERATED_BODY(SButton)

public:
	SButton(const std::wstring& name);
};