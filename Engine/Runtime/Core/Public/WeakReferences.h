// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

namespace SC::Runtime::Core
{
	class Object;

	struct CORE_API WeakReferences
	{
		friend class Object;

	private:
		size_t weakReferences;
		bool bValid : 1;

	public:
		WeakReferences();
		~WeakReferences();

		void AddWeakRef();
		void ReleaseWeak();
		void AddWeakRefInterlocked();
		void ReleaseWeakInterlocked();

		vs_property_get(bool, IsValid);
		bool IsValid_get() const;

	private:
		void Invalidate();
	};
}