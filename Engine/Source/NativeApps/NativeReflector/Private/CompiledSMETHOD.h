// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CompiledSMEMBER.h"

class CompiledSMETHOD : public CompiledSMEMBER
{
public:
	CompiledSMETHOD();

	String ReturnType;
	std::vector<String> Args;
	bool bReadonly = false;
	bool bNoexcept = false;
	bool bVirtual = false;
};