// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Type.h"

template<class T>
class TSubclassOf
{
	template<class>
	friend class TSubclassOf;

	AType* ClassType = nullptr;

public:
	inline TSubclassOf() noexcept
	{
	}

	inline TSubclassOf(const TSubclassOf& Rhs) noexcept
		: ClassType(Rhs.ClassType)
	{
	}

	template<std::derived_from<T> U>
	inline TSubclassOf(const TSubclassOf<U>& Rhs) noexcept
		: ClassType(Rhs.ClassType)
	{
	}

	inline TSubclassOf(AType* InType) noexcept
		: ClassType(InType)
	{
		if (ClassType)
		{
			AType* Comparand = T::StaticClass();
			if (ClassType != Comparand && ClassType->IsDerivedFrom(Comparand) == false)
			{
				ClassType = nullptr;
				check(false);
			}
		}
	}

	inline size_t GetHashCode() const noexcept
	{
		return (size_t)ClassType;
	}

	inline bool IsValid() const noexcept
	{
		return ClassType != nullptr;
	}

	inline AType* GetType() const noexcept
	{
		return ClassType;
	}

	inline TSubclassOf& operator =(const TSubclassOf& Rhs)
	{
		ClassType = Rhs.ClassType;
		return *this;
	}

	template<std::derived_from<T> U>
	inline TSubclassOf& operator =(const TSubclassOf<U>& Rhs)
	{
		ClassType = Rhs.ClassType;
		return *this;
	}

	inline constexpr bool operator ==(const TSubclassOf& Rhs) const noexcept = default;
	inline constexpr auto operator <=>(const TSubclassOf& Rhs) const noexcept = default;
};

template<std::derived_from<AObject> T>
inline RefPtr<T> NewObject(TSubclassOf<T> InClassType)
{
	return NewObject<T>(InClassType.GetType());
}