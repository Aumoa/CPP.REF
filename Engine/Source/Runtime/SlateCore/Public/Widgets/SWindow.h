// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

class NGenericWindow;
class NRHIViewport;

class SLATECORE_API SWindow : public SCompoundWidget
{
private:
	std::shared_ptr<NGenericWindow> NativeWindow;
	std::shared_ptr<NRHIViewport> Viewport;

public:
	SWindow();
	virtual ~SWindow() noexcept override;

	void AttachWindow(std::shared_ptr<NGenericWindow> InNativeWindow);
};