// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Type.h"

namespace libty::inline Core
{
	template<class T>
	class SubclassOf
	{
		template<class>
		friend class SubclassOf;

	private:
		SType* MyType = nullptr;

	public:
		inline SubclassOf() = default;

		inline SubclassOf(const SubclassOf& Rhs) : MyType(Rhs.MyType)
		{
		}

		inline SubclassOf(SubclassOf&& Rhs) : MyType(Rhs.MyType)
		{
			Rhs.MyType = nullptr;
		}

		template<std::derived_from<T> U>
		inline SubclassOf(const SubclassOf<U>& Rhs) : MyType(Rhs.MyType)
		{
		}

		template<std::derived_from<T> U>
		inline SubclassOf(SubclassOf<U>&& Rhs) : MyType(Rhs.MyType)
		{
			Rhs.MyType = nullptr;
		}

		inline SubclassOf(SType* NewType) : MyType(NewType)
		{
			checkf(NewType == nullptr || NewType->IsDerivedFrom(typeof(T)), TEXT("Couldn't convert super class."));
		}

		inline bool IsValid() const
		{
			return MyType != nullptr;
		}

		inline SType* Get()
		{
			return MyType;
		}

		inline SubclassOf& operator =(const SubclassOf& Rhs)
		{
			MyType = Rhs.MyType;
			return *this;
		}

		inline SubclassOf& operator =(SubclassOf&& Rhs)
		{
			MyType = Rhs.MyType;
			Rhs.MyType = nullptr;
			return *this;
		}

		template<std::derived_from<T> U>
		inline SubclassOf& operator =(const SubclassOf<U>& Rhs)
		{
			MyType = Rhs.MyType;
			return *this;
		}

		template<std::derived_from<T> U>
		inline SubclassOf& operator =(SubclassOf<U>&& Rhs)
		{
			MyType = Rhs.MyType;
			Rhs.MyType = nullptr;
			return *this;
		}

		inline auto operator ->() const { return MyType; }
		inline auto operator *() const { return MyType; }
		inline auto operator <=>(const SubclassOf& Rhs) const = default;

		inline operator bool() const { return IsValid(); }
		inline operator SType* () const { return MyType; }
	};
}