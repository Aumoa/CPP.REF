// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string>
#include <string_view>
#include "PrimitiveTypes.h"
#include "LogCore.h"
#include "NonCopyable.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "Reflection/ReflectionMacros.h"
#include "Delegates/MulticastEvent.h"

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
/// Represents unit that subobjects are binding and managed.
/// </summary>
class CORE_API SObject : public SObject_Details::SObjectBase
{
	GENERATED_BODY(SObject)

	template<class T>
	friend class GCRoot;
	template<class T>
	friend class WeakObjectPtr;
	friend class GarbageCollector;

private:
	struct WeakReferencePtr
	{
		int32 bValid : 1 = true;
		int32 WeakReferences : 31 = 0;
	};

private:
	uint64 Generation = 0;
	WeakReferencePtr* WeakReferences = nullptr;

public:
	SObject();
	virtual ~SObject() noexcept;

private:
	SObject(const SObject&) = delete;
	SObject(SObject&&) = delete;

public:
	virtual std::wstring ToString();

protected:
	virtual void PostConstruction();
	virtual std::vector<SObject*> GetGCMembers();

public:
	template<class T, class... TArgs>
	static T* NewObject(TArgs&&... InArgs) requires std::constructible_from<T, TArgs...>
	{
		T* Ptr = new T(std::forward<TArgs>(InArgs)...);
		Ptr->PostConstruction();
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
	inline static TTo Cast(TFrom* InValue) requires (!std::derived_from<TTo, SObject>)
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

	void* operator new(size_t);
	void operator delete(void*);

public:
	static class GarbageCollector& GC();
};

#define implements virtual public 
#include "ValueType.h"