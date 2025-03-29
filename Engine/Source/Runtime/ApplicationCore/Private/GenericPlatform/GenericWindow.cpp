// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GenericPlatform/GenericWindow.h"

namespace Ayla
{
	GenericWindow::GenericWindow()
	{
	}

	extern "C" void GenericWindow_Interop_Dispose(void* instancePtr)
	{
		delete (GenericWindow*)instancePtr;
	}

	extern "C" void GenericWindow_Interop_Show(void* instancePtr)
	{
		((GenericWindow*)instancePtr)->Show();
	}

	extern "C" void GenericWindow_Interop_Hide(void* instancePtr)
	{
		((GenericWindow*)instancePtr)->Hide();
	}
}