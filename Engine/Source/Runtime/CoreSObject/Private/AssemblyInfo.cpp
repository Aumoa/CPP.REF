// Copyright 2020-2022 Aumoa.lib. All right reserved.

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

#include "DependencyInjection/ObjectFactory.h"
#include "DependencyInjection/ObjectFactoryBuilder.h"
#include "DependencyInjection/ServiceType.h"
#include "DependencyInjection/IServiceProvider.h"
#include "DependencyInjection/InjectionErrorCode.h"
#include "DependencyInjection/IServiceCollection.h"
#include "DependencyInjection/IHostedService.h"

#include "Reflection/FieldInfoMetadataGenerator.Impl.h"

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

GENERATE_BODY(libty::Core::SObjectFactory);
GENERATE_BODY(libty::Core::SObjectFactory::InjectionInfo);
GENERATE_BODY(libty::Core::SObjectFactoryBuilder);
GENERATE_BODY(libty::Core::EServiceType);
GENERATE_BODY(libty::Core::IServiceProvider);
GENERATE_BODY(libty::Core::EInjectionErrorCode);
GENERATE_BODY(libty::Core::IServiceCollection);
GENERATE_BODY(libty::Core::IHostedService);