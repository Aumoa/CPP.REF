// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Type.h"
#include "Reflection/TypeRegister.h"

namespace Ayla
{
	class RuntimeType : public Type
	{
		const TypeRegister& m_TR;
		std::vector<const ConstructorInfo*> m_Constructors;

	public:
		RuntimeType(const TypeRegister& tr);

		virtual String GetName() const override;
		virtual String GetNamespace() const override;
		virtual String GetFullName() const override;
		virtual const std::type_info* GetTypeInfo() const override;
		virtual std::span<const ConstructorInfo* const> GetConstructors() const override;
	};
}