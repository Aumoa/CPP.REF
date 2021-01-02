// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12Minimal.h"

#include "Logging/LogVerbosity.h"

LogCategoryBase LogD3D12RHI(ELogVerbosity::Verbose, nameof(LogD3D12RHI));

#define ToD3D12_Begin(D3D12Class, RHIClass)\
D3D12Class ToD3D12(RHIClass value)\
{\
	using T = D3D12Class;\
	switch (value)

#define ToD3D12_End\
	return (T)0;\
}

#define ToD3D12_Item(Prefix, Name)\
	case decltype(value)::Name:\
		return Prefix ## _ ## Name;

#define ToRHI_Begin(D3D12Class, RHIClass)\
RHIClass ToD3D12(D3D12Class value)\
{\
	using T = RHIClass;\
	switch (value)

#define ToRHI_End\
	return (T)0;\
}

#define ToRHI_Item(Prefix, Name)\
	case Prefix ## _ ## Name:\
		return T::Name;

bool IsDepthStencilFormat(ERHITextureFormat value)
{
	return false;
}