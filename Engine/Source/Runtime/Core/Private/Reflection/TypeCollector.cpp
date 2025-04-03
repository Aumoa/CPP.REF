// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Reflection/TypeCollector.h"
#include "Threading/Spinlock.h"
#include "Type.h"
#include <vector>
#include <map>

namespace Ayla
{
	struct TypeCollector::Impl
	{
		Spinlock m_Lock;
		std::vector<std::unique_ptr<Type>> m_Types;
		std::map<size_t, Type*> m_TypeDict;

		void AddType(std::unique_ptr<Type> type)
		{
			al_lock(m_Lock)
			{
				m_TypeDict.emplace(type->GetTypeInfo()->hash_code(), type.get());
				m_Types.emplace_back(std::move(type));
			};
		}
	};

	TypeCollector::Impl* TypeCollector::s_Impl;

	void TypeCollector::AddType(std::unique_ptr<Type> type)
	{
		static struct static_constructor
		{
			static_constructor()
			{
				static TypeCollector::Impl s_Impl_;
				s_Impl = &s_Impl_;
			}
		} static_constructor_;

		s_Impl->AddType(std::move(type));
	}

	Type* TypeCollector::FindType(const std::type_info& ti)
	{
		auto it = s_Impl->m_TypeDict.find(ti.hash_code());
		return it == s_Impl->m_TypeDict.end() ? nullptr : it->second;
	}
}