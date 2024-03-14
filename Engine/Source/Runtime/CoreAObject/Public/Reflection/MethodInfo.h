// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Reflection/MemberInfo.h"

class AParameterInfo;

class COREAOBJECT_API AMethodInfo : public AMemberInfo
{
	AYLA_DECLARE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, MethodInfo);
	AYLA_DECLARE_CLASS_TYPEDEFS(Engine, CoreAObject, MethodInfo);

protected:
	AMethodInfo();

public:
	virtual ~AMethodInfo() noexcept override;

	virtual AType* GetReturnType() const = 0;
	virtual std::vector<AParameterInfo*> GetParameters() const = 0;
};