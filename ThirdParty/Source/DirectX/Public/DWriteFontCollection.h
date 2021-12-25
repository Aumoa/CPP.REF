// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DXGIFactoryChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHIFontCollection.h"

class DIRECTX_API SDWriteFontCollection : public SDXGIFactoryChild, implements IRHIFontCollection
{
	GENERATED_BODY(SDWriteFontCollection)

private:
	ComPtr<IDWriteFontCollection> Collection;
	std::vector<ComPtr<IDWriteFontFamily>> Families;

public:
	SDWriteFontCollection(SDXGIFactory* InFactory, ComPtr<IDWriteFontCollection> Collection);

	using Super::Dispose;

protected:
	virtual void Dispose(bool bDisposing);

public:
	DECLARE_GETTER(IDWriteFontCollection, Collection);
};