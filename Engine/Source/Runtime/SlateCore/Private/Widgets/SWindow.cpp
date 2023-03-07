// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SWindow.h"

SWindow::SWindow()
{
}

SWindow::~SWindow() noexcept
{
}

void SWindow::AttachWindow(std::shared_ptr<NGenericWindow> InNativeWindow)
{
	check(!NativeWindow);
	NativeWindow = InNativeWindow;
}