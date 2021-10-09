// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DWriteFontCollection.h"

SDWriteFontCollection::SDWriteFontCollection(SDXGIFactory* factory, ComPtr<IDWriteFontCollection> collection) : Super(factory)
	, _collection(std::move(collection))
{
	uint32 count = _collection->GetFontFamilyCount();
	_families.reserve(count);

	for (uint32 i = 0; i < count; ++i)
	{
		HR(_collection->GetFontFamily(i, &_families.emplace_back()));
	}
}