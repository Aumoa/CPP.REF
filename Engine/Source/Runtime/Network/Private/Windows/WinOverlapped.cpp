// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "WinOverlapped.h"
#include "WinOverlappedContainer.h"
#include <iostream>

void WinOverlapped::AddRef()
{
#if DO_CHECK
	bool exp = false;
	check(bCheckDup.compare_exchange_strong(exp, true));
#endif
	ContainerRef->AddRef();
}

void WinOverlapped::Release()
{
#if DO_CHECK
	bool exp = true;
	check(bCheckDup.compare_exchange_strong(exp, false));
#endif
	ContainerRef->Release();
}

#endif