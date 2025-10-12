// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/MemberInfo.h"
#include <any>
#include <span>

namespace Ayla
{
    class CORE_API MethodBase : public MemberInfo
    {
    protected:
        MethodBase();

    public:
        virtual ~MethodBase() noexcept override;

        virtual bool IsPublic() const = 0;
        virtual bool IsPrivate() const = 0;
        virtual std::any Invoke(std::any obj, std::span<std::any const> parameters) const = 0;
    };
}