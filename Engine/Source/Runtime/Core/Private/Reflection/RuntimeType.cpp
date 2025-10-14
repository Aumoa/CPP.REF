// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Reflection/RuntimeType.h"
#include "Reflection/RuntimeConstructorInfo.h"

namespace Ayla
{
    RuntimeType::RuntimeType(const TypeRegister& tr)
        : m_TR(tr)
    {
        auto defaultConstructor = m_TR.PCollector.GetDefaultConstructor();
        if (defaultConstructor.second)
        {
            m_Constructors.emplace_back(new RuntimeConstructorInfo(defaultConstructor.first, defaultConstructor.second));
        }
    }

    String RuntimeType::GetName() const
    {
        return m_TR.Name;
    }

    String RuntimeType::GetNamespace() const
    {
        return m_TR.Namespace;
    }

    String RuntimeType::GetFullName() const
    {
        return m_TR.FullName;
    }

    const std::type_info* RuntimeType::GetTypeInfo() const
    {
        return m_TR.TypeInfo;
    }

    ManagedTypeWrapper RuntimeType::GetManagedType() const
    {
        return m_TR.ManagedTypeGetter();
	}

    std::span<const ConstructorInfo* const> RuntimeType::GetConstructors() const
    {
        return m_Constructors;
    }
}