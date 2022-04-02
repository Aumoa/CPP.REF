// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/Assembly.h"

#include "CoreMinimal.h"
#include "IDisposable.h"

#include "Attributes/Attribute.h"
#include "Attributes/ClassAttribute.h"
#include "Attributes/FieldATtribute.h"
#include "Attributes/FieldAttributeSerializable.h"
#include "Attributes/MethodAttribute.h"

#include "Reflection/Assembly.h"
#include "Reflection/Enum.h"
#include "Reflection/FieldInfo.h"
#include "Reflection/MemberInfo.h"
#include "Reflection/MethodInfo.h"
#include "Reflection/Type.h"

DEFINE_ASSEMBLY_INFO;

GENERATE_BODY(IDisposable);

GENERATE_BODY(SAttribute);
GENERATE_BODY(SClassAttribute);
GENERATE_BODY(SFieldAttribute);
GENERATE_BODY(SFieldAttributeSerializable);
GENERATE_BODY(SMethodAttribute);

GENERATE_BODY(SAssembly);
GENERATE_BODY(SEnum);
GENERATE_BODY(SFieldInfo);
GENERATE_BODY(SMemberInfo);
GENERATE_BODY(SMethodInfo);
GENERATE_BODY(SType);