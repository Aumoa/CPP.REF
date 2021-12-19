// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string>
#include <string_view>
#include <atomic>
#include "PrimitiveTypes.h"
#include "LogCore.h"
#include "NonCopyable.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "Reflection/ReflectionMacros.h"

class SValueType;

namespace SObject_Details
{
	class CORE_API SObjectBase : public NonCopyable
	{
		virtual Type* GetType() const = 0;
	};
}

/// <summary>
/// Supports all classes in the smart component hierarchy and provides low-level services to derived classes.
/// </summary>
class CORE_API SObject : public SObject_Details::SObjectBase
{
	GENERATED_BODY(SObject)

	template<class T>
	friend class SharedPtr;
	template<class T>
	friend class WeakPtr;
	friend class GarbageCollector;

private:
	struct WeakReferencePtr
	{
		std::atomic<bool> bDisposed = false;
		std::atomic<int32> References = 0;
		std::atomic<int32> WeakReferences = 0;

		inline bool IsValid() const volatile
		{
			return !bDisposed;
		}
	};

private:
	uint64 Generation = 0;
	size_t InternalIndex = -1;
	WeakReferencePtr* ReferencePtr = nullptr;

public:
	SObject();
	virtual ~SObject() noexcept;

	void AddToRoot();
	void RemoveFromRoot();

private:
	SObject(const SObject&) = delete;
	SObject(SObject&&) = delete;

	void MarkGC(uint64 Generation);
	void UnmarkGC();

public:
	virtual std::wstring ToString();

protected:
	virtual void PostConstruction();

public:
	template<class T, class... TArgs>
	static T* NewObject(TArgs&&... InArgs) requires std::constructible_from<T, TArgs...>
	{
		T* Ptr = new T(std::forward<TArgs>(InArgs)...);
		((SObject*)Ptr)->PostConstruction();
		return Ptr;
	}

	template<std::derived_from<SObject> TTo, std::derived_from<SObject> TFrom>
	inline static TTo* Cast(TFrom* InFrom)
	{
		return dynamic_cast<TTo*>(InFrom);
	}

	template<std::same_as<SObject> TTo, class TFrom>
	inline static TTo* Cast(const TFrom& InValue) requires (!std::derived_from<TFrom, SObject>)
	{
		return NewObject<SValueType>(InValue);
	}

	template<class TTo, std::same_as<SObject> TFrom>
	inline static TTo Cast(TFrom* InValue) requires (!std::derived_from<TTo, SObject>) && (!requires(const TTo& IsOpt)
	{
		{ IsOpt.has_value() } -> std::same_as<bool>;
		{ *IsOpt };
	})
	{
		auto ValueTypePtr = Cast<SValueType>(InValue);
		if (ValueTypePtr == nullptr)
		{
			SE_LOG(LogCasts, Fatal, L"Object is not boxing class.");
		}

		TTo OutValue;
		if (!ValueTypePtr->Unboxing(&OutValue))
		{
			SE_LOG(LogCasts, Fatal, L"The type of value contained at boxing object is not match with desired type.");
		}

		return OutValue;
	}

	template<class TTo, std::same_as<SObject> TFrom>
	inline static TTo Cast(TFrom* InValue) requires (!std::derived_from<TTo, SObject>) && requires(const TTo& IsOpt)
	{
		{ IsOpt.has_value() } -> std::same_as<bool>;
		{ *IsOpt };
	}
	{
		using ValueT = std::remove_reference_t<decltype(*std::declval<TTo>())>;

		auto ValueTypePtr = Cast<SValueType>(InValue);
		if (ValueTypePtr == nullptr)
		{
			return std::nullopt;
		}

		ValueT OutValue;
		if (!ValueTypePtr->Unboxing(&OutValue))
		{
			return std::nullopt;
		}

		return OutValue;
	}

	void* operator new(size_t);
	void operator delete(void*);
};

#define implements virtual public 
#include "ValueType.h"