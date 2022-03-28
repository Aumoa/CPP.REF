// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"

class CORESOBJECT_API SAttribute : virtual public SObject
{
	GENERATED_BODY(SAttribute);

public:
	SAttribute();
	SAttribute(SAttribute&&);
};