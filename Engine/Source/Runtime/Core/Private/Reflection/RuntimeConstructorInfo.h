// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/ConstructorInfo.h"
#include "Reflection/reflexpr.h"
#include <memory>

namespace Ayla
{
    class Object;

    class RuntimeConstructorInfo : public ConstructorInfo
    {
    private:
        std::reflect::access_type m_Access;
        std::shared_ptr<Object>(*const m_Function)();

    public:
        RuntimeConstructorInfo(std::reflect::access_type access, std::shared_ptr<Object>(*function)());
        virtual ~RuntimeConstructorInfo() noexcept override;

        virtual bool IsPublic() const override;
        virtual bool IsPrivate() const override;
        virtual std::any Invoke(std::any obj, std::span<std::any const> parameters) const override;

        virtual std::any Invoke(std::span<std::any const> parameters) const override;
    };
}