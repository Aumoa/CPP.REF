// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIGraphics.h"
#include "RHI/RHICommon.h"

#if PLATFORM_WINDOWS
#include "D3D12RHI/D3D12Graphics.h"
#endif

NRHIGraphics::NRHIGraphics()
{
}

NRHIGraphics::~NRHIGraphics() noexcept
{
}

std::unique_ptr<NRHIGraphics> NRHIGraphics::GenerateGraphics(String InRHIModuleName)
{
#if PLATFORM_WINDOWS
	if (InRHIModuleName == TEXT("D3D12"))
	{
		return ND3D12Graphics::GenerateGraphics();
	}
#endif

	Log::Fatal(LogRHI, TEXT("Cannot initialize graphics. Cannot found RHI module `{}`."), InRHIModuleName);
}