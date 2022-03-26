// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"

/// <summary>
/// Obtains information about the attributes of a member and provides access to member metadata.
/// </summary>
class CORESOBJECT_API SMemberInfo : implements SObject
{
	GENERATED_BODY(SMemberInfo);

protected:
	SMemberInfo();

public:
	/// <summary>
	/// Gets the name of current member.
	/// </summary>
	virtual std::wstring GetName() = 0;
};