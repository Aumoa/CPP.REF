// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

class NonCopyable
{
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator =(const NonCopyable&) = delete;

public:
	NonCopyable()
	{
	}
};