// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Reflection/MemberInfo.h"

class AParameterInfo;

class COREAOBJECT_API AMethodInfo : public AMemberInfo
{
protected:
	AMethodInfo()
	{
	}

public:
	virtual ~AMethodInfo() noexcept
	{
	}

	virtual AType* GetReturnType() const = 0;
	virtual std::vector<AParameterInfo*> GetParameters() const = 0;
};