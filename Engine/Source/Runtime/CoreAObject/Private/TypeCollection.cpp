// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAObject/TypeCollection.h"
#include "Type.h"

inline namespace TypeCollectionVars
{
	static std::vector<AType*>& Collection()
	{
		static std::vector<AType*> StaticInitVar;
		return StaticInitVar;
	}
}

std::span<AType* const> NTypeCollection::GetTypeCollection()
{
	return Collection();
}

void NTypeCollection::AddClassTypeToCollection(AType* InClassType)
{
	Collection().emplace_back(InClassType);
}