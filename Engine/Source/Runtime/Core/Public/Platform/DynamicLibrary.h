// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Action.h"
#include "Func.h"

namespace Ayla
{
	class CORE_API DynamicLibrary
	{
		struct Implementation;
		DynamicLibrary(const DynamicLibrary&) = delete;
		DynamicLibrary& operator =(const DynamicLibrary&) = delete;

	private:
		String LibraryName;
		Implementation* Impl;

	public:
		DynamicLibrary();
		DynamicLibrary(String InLibraryName);
		DynamicLibrary(DynamicLibrary&& rhs) noexcept;
		~DynamicLibrary() noexcept;

		bool IsValid() const;
		String GetName() const;
		void Detach();

		template<class... TArgs>
		Action<TArgs...> LoadAction(String Signature) const
		{
			void (*Ptr)() = InternalLoadFunction(Signature);
			if (Ptr == nullptr)
			{
				return {};
			}

			return Action<TArgs...>(reinterpret_cast<void(*)(TArgs...)>(Ptr));
		}

		template<class... TArgs>
		Func<TArgs...> LoadFunction(String Signature) const
		{
			void (*Ptr)() = InternalLoadFunction(Signature);
			if (Ptr == nullptr)
			{
				return {};
			}

			return Func<TArgs...>::FromAnonymous(Ptr);
		}

		template<class T>
		T LoadDelegate(String signature) const
		{
			void (*ptr)() = InternalLoadFunction(signature);
			if (ptr == nullptr)
			{
				return {};
			}

			return reinterpret_cast<T>(ptr);
		}

		DynamicLibrary& operator =(DynamicLibrary&& rhs) noexcept;

	private:
		void (*InternalLoadFunction(String Signature) const)();
	};
}