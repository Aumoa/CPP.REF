// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "Object.h"
#include <typeinfo>

namespace Ayla
{
	class GC;
	class PropertyCollector;

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

	protected:
		virtual const PropertyCollector* GetPropertyCollector() const = 0;
	};
}