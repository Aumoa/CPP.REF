// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Reflection/RuntimeConstructorInfo.h"
#include "InvalidOperationException.h"
#include "Object.h"

namespace Ayla
{
    RuntimeConstructorInfo::RuntimeConstructorInfo(std::reflect::access_type access, SharedPtr<Object>(*function)())
        : m_Access(access)
        , m_Function(function)
    {
    }

    RuntimeConstructorInfo::~RuntimeConstructorInfo() noexcept
    {
    }

    bool RuntimeConstructorInfo::IsPublic() const
    {
        return m_Access == std::reflect::access_type::public_;
    }

    bool RuntimeConstructorInfo::IsPrivate() const
    {
        return m_Access == std::reflect::access_type::private_;
    }

    std::any RuntimeConstructorInfo::Invoke(std::any obj, std::span<std::any const> parameters) const
    {
        if (obj.has_value())
        {
            throw InvalidOperationException(TEXT("Constructor cannot be invoked on an existing instance"));
        }
        
        return Invoke(parameters);
    }

    std::any RuntimeConstructorInfo::Invoke(std::span<std::any const> parameters) const
    {
        // TODO:
        if (!parameters.empty())
        {
            throw InvalidOperationException(TEXT("Parameterized constructors not yet supported"));
        }

        return m_Function();
    }
}