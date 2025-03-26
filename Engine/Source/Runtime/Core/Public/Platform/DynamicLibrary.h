// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Action.h"
#include "Func.h"

namespace Ayla
{
	class CORE_API DynamicLibrary
	{
		class Implementation;

	private:
		String LibraryName;
		Implementation* Impl;

	public:
		DynamicLibrary();
		DynamicLibrary(String InLibraryName);
		~DynamicLibrary() noexcept;

		bool IsValid() const;

		template<class... TArgs>
		Action<TArgs...> LoadAction(String Signature)
		{
			void (*Ptr)() = InternalLoadFunction(Signature);
			if (Ptr == nullptr)
			{
				return {};
			}

			return Action<TArgs...>(reinterpret_cast<void(*)(TArgs...)>(Ptr));
		}

		template<class... TArgs>
		Func<TArgs...> LoadFunction(String Signature)
		{
			void (*Ptr)() = InternalLoadFunction(Signature);
			if (Ptr == nullptr)
			{
				return {};
			}

			return Func<TArgs...>::FromAnonymous(Ptr);
		}

	private:
		void (*InternalLoadFunction(String Signature))();
	};
}