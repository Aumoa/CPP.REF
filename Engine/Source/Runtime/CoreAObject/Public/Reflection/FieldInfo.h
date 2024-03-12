// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Reflection/MemberInfo.h"

class COREAOBJECT_API AFieldInfo : public AMemberInfo
{
protected:
	AFieldInfo()
	{
	}

public:
	virtual ~AFieldInfo() noexcept
	{
	}

	virtual AType* GetFieldType() const = 0;
};