// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SLATECORE_API SWidget : public NonCopyable
{
public:
	SWidget();
	virtual ~SWidget() noexcept;
};