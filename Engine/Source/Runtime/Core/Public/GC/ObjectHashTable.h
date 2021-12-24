// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <vector>

class SObject;

class CORE_API ObjectHashTable
{
	std::vector<SObject*> Collection;
	std::vector<size_t> IndexPool;
	size_t IndexPoolSize = 0;
	size_t NumPoolCompact = 0;

public:
	inline ObjectHashTable()
	{
	}

	SObject*& Emplace(SObject* InObject);
	void Remove(SObject* InObject);

	inline size_t NumObjects() const
	{
		return Collection.size() - (IndexPoolSize - NumPoolCompact);
	}

	inline SObject* Get(size_t IndexOf) const
	{
		return Collection[IndexOf];
	}

	inline void Set(size_t IndexOf, SObject* Value)
	{
		Collection[IndexOf] = Value;
	}

	inline void Clear()
	{
		Collection.clear();
		IndexPool.clear();
	}

	size_t TableSize() const
	{
		return Collection.size();
	}

	void CompactIndexTable(size_t LiveIndex, std::vector<SObject*>& PendingKill);

	[[nodiscard]] inline auto size() const
	{
		return NumObjects();
	}

	[[nodiscard]] inline auto begin() const
	{
		return Collection.begin();
	}

	[[nodiscard]] inline auto end() const
	{
		return Collection.end();
	}
};