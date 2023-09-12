// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "WindowsCo.h"

#if PLATFORM_WINDOWS

#include "WindowsCommon.h"

void NWindowsCoStatics::TryCoInitialize()
{
	static int _ = ([]() { HR(::CoInitializeEx(NULL, COINIT_MULTITHREADED)) }(), 0);
}

ComPtr<IWICImagingFactory2> NWindowsCoStatics::GetImagingFactory()
{
	static ComPtr<IWICImagingFactory2> sInstance = []()
	{
		TryCoInitialize();
		ComPtr<IWICImagingFactory2> pInstance;
		HR(::CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pInstance)));
		return std::move(pInstance);
	}();
	return sInstance;
}

#endif