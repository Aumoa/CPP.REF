// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Type.h"
#include "Reflection/TypeRegister.h"

namespace Ayla
{
	class RuntimeType : public Type
	{
		const TypeRegister& m_TR;

	public:
		RuntimeType(const TypeRegister& tr)
			: m_TR(tr)
		{
		}

		virtual String GetName() const override
		{
			return m_TR.Name;
		}

		virtual String GetNamespace() const override
		{
			return m_TR.Namespace;
		}

		virtual String GetFullName() const override
		{
			return m_TR.FullName;
		}

		virtual const std::type_info* GetTypeInfo() const override
		{
			return m_TR.TypeInfo;
		}

	protected:
		virtual const PropertyCollector* GetPropertyCollector() const override
		{
			return &m_TR.PCollector;
		}
	};
}