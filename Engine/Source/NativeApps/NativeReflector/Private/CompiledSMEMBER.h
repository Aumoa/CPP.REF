// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "BodyAccessKey.h"
#include "Misc/StringBuilder.h"

class CompiledSMEMBER
{
public:
	CompiledSMEMBER();

	String MemberName;
	bool bInline = false;
	bool bStatic = false;
	EBodyAccessKey eBodyAccessKey = EBodyAccessKey::Private;

	virtual void Generate(const String& owningType, size_t indent, StringBuilder& sb);
};