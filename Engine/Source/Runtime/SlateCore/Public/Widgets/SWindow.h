// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

class NGenericWindow;

class SLATECORE_API SWindow : public SCompoundWidget
{
private:
	std::shared_ptr<NGenericWindow> NativeWindow;

public:
	SWindow();
	virtual ~SWindow() noexcept override;

	void AttachWindow(std::shared_ptr<NGenericWindow> InNativeWindow);
};