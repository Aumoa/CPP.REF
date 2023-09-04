// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SLATECORE_API SWidget
{
	SWidget(const SWidget&) = delete;
	SWidget(SWidget&&) = delete;

public:
	SWidget();
	virtual ~SWidget() noexcept;
};