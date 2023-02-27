// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/NonCopyable.h"

class SLATECORE_API SWidget : public NonCopyable
{
public:
	SWidget();
	virtual ~SWidget() noexcept;
};