// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <thread>
#include <functional>
#include <stop_token>
#include <vector>

class CORE_API ThreadGroup
{
	ThreadGroup(const ThreadGroup&) = delete;
	ThreadGroup& operator =(const ThreadGroup&) = delete;

private:
	String _groupName;
	std::vector<std::jthread> _threads;

public:
	using Worker_t = std::function<void(size_t index, std::stop_token)>;

public:
	ThreadGroup(const String& groupName);
	virtual ~ThreadGroup() noexcept;

	void CreateThread(Worker_t body);
	void StopAll();
	void JoinAll();
};