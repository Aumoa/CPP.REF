// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/MethodBase.h"

namespace Ayla
{
    class CORE_API ConstructorInfo : public MethodBase
    {
    protected:
        ConstructorInfo();

    public:
        virtual ~ConstructorInfo() noexcept override;

        virtual std::any Invoke(std::span<std::any const> parameters) const = 0;
    };
}