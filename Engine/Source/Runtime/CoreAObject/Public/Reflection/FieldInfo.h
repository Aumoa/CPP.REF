// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Reflection/MemberInfo.h"

class COREAOBJECT_API AFieldInfo : public AMemberInfo
{
	AYLA_DECLARE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, FieldInfo);
	AYLA_DECLARE_CLASS_TYPEDEFS(Engine, CoreAObject, FieldInfo);

protected:
	AFieldInfo();

public:
	virtual ~AFieldInfo() noexcept override;

	virtual AType* GetFieldType() const = 0;
};