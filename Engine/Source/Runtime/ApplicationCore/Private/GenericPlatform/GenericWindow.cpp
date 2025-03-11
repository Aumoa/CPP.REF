// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GenericPlatform/GenericWindow.h"

NGenericWindow::NGenericWindow()
{
}

NGenericWindow::~NGenericWindow() noexcept
{
}

extern "C" void GenericWindow_Interop_Dispose(void* instancePtr)
{
	delete (NGenericWindow*)instancePtr;
}

extern "C" void GenericWindow_Interop_Show(void* instancePtr)
{
	((NGenericWindow*)instancePtr)->Show();
}

extern "C" void GenericWindow_Interop_Hide(void* instancePtr)
{
	((NGenericWindow*)instancePtr)->Hide();
}