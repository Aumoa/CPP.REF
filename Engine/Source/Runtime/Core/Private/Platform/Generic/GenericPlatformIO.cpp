// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "Platform/PlatformMacros.h"

export module Core:GenericPlatformIO;

export import :StaticClass;

export class CORE_API GenericPlatformIO : public StaticClass
{
public:
	static constexpr size_t OVERLAPPED_SIZE = 1;
};