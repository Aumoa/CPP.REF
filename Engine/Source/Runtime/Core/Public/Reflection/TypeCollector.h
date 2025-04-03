// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "StaticClass.h"
#include "Platform/PlatformMacros.h"
#include <memory>

namespace Ayla
{
	class Type;

	struct CORE_API TypeCollector : public StaticClass
	{
	private:
		struct Impl;
		static Impl* s_Impl;

	public:
		static void AddType(std::unique_ptr<Type> type);
		static Type* FindType(const std::type_info& ti);
	};
}