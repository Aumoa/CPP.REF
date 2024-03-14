// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"

class AType;

class COREAOBJECT_API AMemberInfo : public AObject
{
	AYLA_DECLARE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, MemberInfo);
	AYLA_DECLARE_CLASS_TYPEDEFS(Engine, CoreAObject, MemberInfo);

protected:
	AMemberInfo();

public:
	virtual ~AMemberInfo() noexcept override;

	virtual AType* GetDeclaringType() const = 0;
};