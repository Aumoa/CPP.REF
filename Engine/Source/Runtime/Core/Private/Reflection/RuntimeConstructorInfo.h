// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/ConstructorInfo.h"
#include "Reflection/reflexpr.h"
#include "SharedPtr.h"
#include <memory>

namespace Ayla
{
    class Object;

    class RuntimeConstructorInfo : public ConstructorInfo
    {
    private:
        std::experimental::reflect::access_type m_Access;
        SharedPtr<Object>(*const m_Function)();

    public:
        RuntimeConstructorInfo(std::experimental::reflect::access_type access, SharedPtr<Object>(*function)());
        virtual ~RuntimeConstructorInfo() noexcept override;

        virtual bool IsPublic() const override;
        virtual bool IsPrivate() const override;
        virtual std::any Invoke(std::any obj, std::span<std::any const> parameters) const override;

        virtual std::any Invoke(std::span<std::any const> parameters) const override;
    };
}