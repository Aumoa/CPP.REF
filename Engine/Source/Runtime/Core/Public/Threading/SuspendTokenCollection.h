// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <set>
#include <mutex>

class ISuspendToken;

class CORE_API SuspendTokenCollection
{
	SuspendTokenCollection() = delete;

private:
	static std::set<ISuspendToken*> _tokens;
	static std::mutex _lock;

public:
	static bool Add(ISuspendToken* token);
	static bool Remove(ISuspendToken* token);
	static bool Contains(ISuspendToken* token);
	static const std::set<ISuspendToken*>& Collection();
};