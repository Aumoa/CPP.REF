// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <set>
#include <string>
#include <string_view>
#include <functional>
#include <map>
#include <functional>
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

private:
	std::wstring Name;
	uint64 Generation;
	std::shared_ptr<bool> WeakReferences;

	SPROPERTY(Outer)
	SObject* Outer = nullptr;

public:
	SObject();
	virtual ~SObject() noexcept;

private:
	SObject(const SObject&) = delete;
	SObject(SObject&&) = delete;

public:
	virtual std::wstring ToString();

	void SetName(std::wstring_view InNewName);
	std::wstring GetName();
	SObject* GetOuter();

protected:
	virtual void PostConstruction();

public:
	template<class T, class... TArgs>
	static T* NewObject(TArgs&&... InArgs) requires std::constructible_from<T, TArgs...>
	{
		T* Ptr = new T(std::forward<TArgs>(InArgs)...);
		Ptr->PostConstruction();
		return Ptr;
	}

	template<class T, class... TArgs>
	T* NewSubobject(TArgs&&... InArgs) requires std::constructible_from<T, TArgs...>
	{
		T* Ptr = new T(std::forward<TArgs>(InArgs)...);
		Ptr->Outer = this;
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
	class CORE_API GarbageCollector
	{
		template<class T>
		friend class GCRoot;
		friend class SObject;

		std::set<SObject*> Collection;
		std::set<SObject*> Roots;
		uint64 Generation = 0;

	private:
		GarbageCollector();

	public:
		void Collect(bool bLog = false);
		size_t NumThreadObjects();
		void SuppressFinalize(SObject* Object);

		void Consume(GarbageCollector& AnotherThreadGC);
	};

	static GarbageCollector& GC();
	void MarkAndSweep(uint64 Generation);
};

#define implements virtual public 
#include "ValueType.h"