// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CompiledSMEMBER.h"

class CompiledSCONSTRUCTOR : public CompiledSMEMBER
{
public:
	CompiledSCONSTRUCTOR();

	std::vector<String> Args;
	bool bNoexcept = false;
};