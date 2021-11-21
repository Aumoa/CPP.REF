// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <set>
#include <string>
#include <string_view>
#include <functional>
#include <map>
#include <mutex>
#include "PrimitiveTypes.h"
#include "LogCore.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "Reflection/ReflectionMacros.h"
#include "Delegates/MulticastEvent.h"

class SValueType;

/// <summary>
/// Supports all classes in the smart component hierarchy and provides low-level services to derived classes.
/// Represents unit that subobjects are binding and managed.
/// </summary>
class CORE_API SObject : public std::enable_shared_from_this<SObject>
{
	GENERATED_BODY(SObject)
	friend class ObjectId;
	friend class Type;

private:
	static std::atomic<uint64> InternalObjectIndexGenerator;
	const uint64 Index;

	SObject* Outer = nullptr;

	std::vector<std::shared_ptr<SObject>> Subobjects;
	std::map<SObject*, size_t> Views;

	std::wstring Name;
	std::mutex ObjectLock;

public:
	SObject();
	virtual ~SObject() noexcept;

private:
	SObject(const SObject&) = delete;
	SObject(SObject&&) = delete;

public:
	virtual std::wstring ToString(std::wstring_view InFormatArgs = L"");

	SObject* GetOuter() const;
	std::shared_ptr<SObject> SetOuter(SObject* InNewOuter);
	void SetName(std::wstring_view InNewName);
	std::wstring GetName();

	void AddReferenceObject(SObject* InObject);
	void RemoveReferenceObject(SObject* InObject);

	void DestroyObject(SObject* InObject);
	void CleanupSubobjects();

	DECLARE_MULTICAST_EVENT(OuterChangedEvent, SObject* This);
	OuterChangedEvent OuterChanged;
	DECLARE_MULTICAST_EVENT(NameChangedEvent, SObject* This);
	NameChangedEvent NameChanged;

public:
	template<class T, class... TArgs>
	T* NewObject(TArgs&&... InArgs) requires std::constructible_from<T, TArgs...>
	{
		std::unique_lock Lock(ObjectLock);
		std::shared_ptr SharedPtr = std::make_shared<T>(std::forward<TArgs>(InArgs)...);
		auto Ptr = SharedPtr.get();
		InternalAttachSubobject(Ptr);
		InternalAttachObjectName(Ptr);
		Ptr->Outer = this;
		return Ptr;
	}

	template<std::derived_from<SObject> TTo, std::derived_from<SObject> TFrom>
	inline static TTo* Cast(TFrom* InFrom)
	{
		return dynamic_cast<TTo*>(InFrom);
	}

	template<std::same_as<SObject> TTo, class TFrom>
	inline TTo* Cast(const TFrom& InValue) requires (!std::derived_from<TFrom, SObject>)
	{
		return NewObject<SValueType>(InValue);
	}

	template<class TTo, std::same_as<SObject> TFrom>
	static inline TTo Cast(TFrom* InValue) requires (!std::derived_from<TTo, SObject>)
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

	template<class T, class... TArgs>
	static std::shared_ptr<T> MakeShared(TArgs&&... InArgs) requires std::constructible_from<T, TArgs...>
	{
		std::shared_ptr SharedPtr = std::make_shared<T>(std::forward<TArgs>(InArgs)...);
		InternalAttachObjectName(SharedPtr.get());
		return SharedPtr;
	}

	void* operator new(size_t);
	void operator delete(void*);

private:
	void InternalDetachSubobject(SObject* Subobject);
	void InternalAttachSubobject(SObject* Subobject);
	static void InternalAttachObjectName(SObject* InObject);
};

#define implements virtual public 
#include "ValueType.h"