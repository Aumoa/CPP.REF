// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/SuspendTokenCollection.h"

using namespace libty;

std::set<ISuspendToken*> SuspendTokenCollection::_tokens;
std::mutex SuspendTokenCollection::_lock;

bool SuspendTokenCollection::Add(ISuspendToken* token)
{
	std::unique_lock lock(_lock);
	auto it = _tokens.emplace(token);
	return it.second;
}

bool SuspendTokenCollection::Remove(ISuspendToken* token)
{
	std::unique_lock lock(_lock);
	return _tokens.erase(token) > 0;
}

bool SuspendTokenCollection::Contains(ISuspendToken* token)
{
	std::unique_lock lock(_lock);
	return _tokens.contains(token);
}

const std::set<ISuspendToken*>& SuspendTokenCollection::Collection()
{
	return _tokens;
}