// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DXGIFactoryChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHIFontCollection.h"

class SDWriteFontCollection : public SDXGIFactoryChild, implements IRHIFontCollection
{
	GENERATED_BODY(SDWriteFontCollection)

private:
	ComPtr<IDWriteFontCollection> _collection;
	std::vector<ComPtr<IDWriteFontFamily>> _families;

public:
	SDWriteFontCollection(SDXGIFactory* factory, ComPtr<IDWriteFontCollection> collection);

public:
	DECLARE_GETTER(IDWriteFontCollection, _collection);
};