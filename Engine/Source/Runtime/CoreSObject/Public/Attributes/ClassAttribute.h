// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Attribute.h"

class CORESOBJECT_API SClassAttribute : public SAttribute
{
	GENERATED_BODY(SClassAttribute);

private:
	SType* _bindType = nullptr;

public:
	SClassAttribute();

	virtual void SetBindType(SType* bindType);
	virtual SType* GetBindType();
};