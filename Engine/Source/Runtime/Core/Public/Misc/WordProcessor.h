// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CORE_API WordProcessor
{
public:
	WordProcessor();

	std::function<bool(char)> IsSeparator;
	std::function<bool(char)> IsSpace;
	std::function<bool(char)> IsNumber;

	uint8 bProcessString : 1 = true;

	bool Process(std::span<const uint8> Input, std::function<bool(std::string_view, size_t, size_t)> Processor);
};