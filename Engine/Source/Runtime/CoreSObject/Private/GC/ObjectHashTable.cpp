// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GC/ObjectHashTable.h"
#include "Object.h"
#include "CoreAssert.h"

SObject*& ObjectHashTable::Emplace(SObject* InObject)
{
	check(InObject->InternalIndex == -1);

	if (IndexPoolSize)
	{
		size_t Index = IndexPool[--IndexPoolSize];
		// Consume over indexes.
		while (Index >= Collection.size())
		{
			NumPoolCompact -= 1;
		}
		SObject*& Ref = Collection[Index] = InObject;
		InObject->InternalIndex = Index;
		return Ref;
	}

	size_t Idx = Collection.size();
	SObject*& Ref = Collection.emplace_back(InObject);
	IndexPool.emplace_back();
	InObject->InternalIndex = Idx;
	return Ref;
}

void ObjectHashTable::Remove(SObject* InObject)
{
	if (InObject->InternalIndex != -1)
	{
		size_t Index = InObject->InternalIndex;
		Collection[InObject->InternalIndex] = nullptr;
		IndexPool[IndexPoolSize++] = Index;
		InObject->InternalIndex = -1;
	}
}

void ObjectHashTable::CompactIndexTable(size_t LiveIndex, std::vector<SObject*>& PendingKill)
{
	for (SObject*& Object : PendingKill)
	{
		IndexPool[IndexPoolSize++] = Object->InternalIndex;
	}

	NumPoolCompact += Collection.size() - (LiveIndex + 1);
	Collection.resize(LiveIndex + 1);

	if (Collection.size())
	{
		size_t BackIterator = Collection.size() - 1;
		while (IndexPoolSize && Collection[BackIterator] && BackIterator != -1)
		{
			size_t Index = IndexPool[--IndexPoolSize];
			if (Index >= Collection.size())
			{
				NumPoolCompact -= 1;
				continue;
			}

			Collection[BackIterator]->InternalIndex = Index;
			std::swap(Collection[Index], Collection[BackIterator--]);
		}

		Collection.resize(BackIterator + 1);
	}
}