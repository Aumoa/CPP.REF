// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string>
#include <string_view>
#include <atomic>
#include "PrimitiveTypes.h"
#include "LogCore.h"
#include "NonCopyable.h"
#include "Reflection/ReflectionMacros.h"
#include "Reflection/Type.h"
#include "Casts.h"
#include "GC/Referencer.h"

class SValueType;

namespace SObject_Details
{
	class CORE_API SObjectBase
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
	friend class ObjectHashTable;

private:
	uint64 Generation = 0;
	size_t InternalIndex = -1;
	Referencer* ReferencePtr = nullptr;

public:
	SObject();
	SObject(SObject&& Rhs);
	virtual ~SObject() noexcept;

	void AddToRoot();
	void RemoveFromRoot();

private:
	SObject(const SObject&) = delete;

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
		SObject* Ptr = new T(std::forward<TArgs>(InArgs)...);
		Ptr->PostConstruction();
		return dynamic_cast<T*>(Ptr);
	}

	void* operator new(size_t);
	void operator delete(void*);
};

#define implements virtual public 