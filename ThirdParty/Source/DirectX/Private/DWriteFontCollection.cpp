// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DWriteFontCollection.h"

GENERATE_BODY(SDWriteFontCollection);

SDWriteFontCollection::SDWriteFontCollection(SDXGIFactory* InFactory, ComPtr<IDWriteFontCollection> Collection) : Super(InFactory)
	, Collection(std::move(Collection))
{
	uint32 Count = Collection->GetFontFamilyCount();
	Families.reserve(Count);

	for (uint32 i = 0; i < Count; ++i)
	{
		HR(Collection->GetFontFamily(i, &Families.emplace_back()));
	}
}

void SDWriteFontCollection::Dispose(bool bDisposing)
{
	Collection.Reset();
	Families.clear();

	Super::Dispose(bDisposing);
}