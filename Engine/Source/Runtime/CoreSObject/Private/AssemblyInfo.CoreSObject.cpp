// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "AssemblyInfo.CoreSObject.h"

DEFINE_ASSEMBLY_INFO;

GENERATE_BODY(libty::Core::IDisposable);
GENERATE_BODY(libty::Core::SType);

GENERATE_BODY(libty::Core::Attributes::SAttribute);
GENERATE_BODY(libty::Core::Attributes::SClassAttribute);
GENERATE_BODY(libty::Core::Attributes::SFieldAttribute);
GENERATE_BODY(Attributes::Core::SFieldAttributeSerializable);
GENERATE_BODY(libty::Core::Attributes::SMethodAttribute);

GENERATE_BODY(libty::Core::Reflection::SAssembly);
GENERATE_BODY(libty::Core::Reflection::SEnum);
GENERATE_BODY(libty::Core::Reflection::SFieldInfo);
GENERATE_BODY(libty::Core::Reflection::SMemberInfo);
GENERATE_BODY(libty::Core::Reflection::SMethodInfo);