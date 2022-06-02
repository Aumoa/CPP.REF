// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <memory>
#include <reflexpr.h>

class Type;

class CORE_API Object
{
public:
	Object() noexcept;
	virtual ~Object() noexcept;

	template<class TSelf>
	Type* GetType(this const TSelf&);  // in Type.h
	virtual String ToString() const noexcept;
};