// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "COM/COMDeviceBundle.h"

#include "COM/COMMinimal.h"
#undef interface
#include <wincodec.h>

COMDeviceBundle::COMDeviceBundle() : Super()
{
	Initialize();
}

COMDeviceBundle::~COMDeviceBundle()
{

}

IWICImagingFactory* COMDeviceBundle::GetImagingFactory() const
{
	return imagingFactory.Get();
}

void COMDeviceBundle::Initialize()
{
	// Initialize COM.
	HR(::CoInitializeEx(nullptr, COINIT_MULTITHREADED));

	// Create WIC imaging factory.
	HR(::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&imagingFactory)));
}