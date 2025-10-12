// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "Object.h"
#include <typeinfo>
#include <span>

namespace Ayla
{
	class GC;
	class ConstructorInfo;

	class Type
	{
		friend GC;

	protected:
		Type() = default;

	public:
		virtual ~Type() noexcept = default;

	public:
		virtual String GetName() const = 0;
		virtual String GetNamespace() const = 0;
		virtual String GetFullName() const = 0;
		virtual const std::type_info* GetTypeInfo() const = 0;
		virtual std::span<const ConstructorInfo* const> GetConstructors() const = 0;
	};
}