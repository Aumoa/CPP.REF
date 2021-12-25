// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string>
#include <string_view>
#include <atomic>
#include "PrimitiveTypes.h"
#include "LogCore.h"
#include "NonCopyable.h"
#include "Casts.h"
#include "ObjectBase.h"
#include "Reflection/ReflectionMacros.h"
#include "Reflection/Type.h"
#include "GC/Referencer.h"

class SValueType;

/// <summary>
/// Supports all classes in the smart component hierarchy and provides low-level services to derived classes.
/// </summary>
class CORE_API SObject : public SObjectDetails::SObjectBase
{
	GENERATED_BODY(SObject)

	template<class T>
	friend class SharedPtr;
	template<class T>
	friend class WeakPtr;
	friend class GarbageCollector;
	friend class ObjectHashTable;
	friend class SObjectDetails::GCNewBinder;

private:
	bool bMarkAtGC = false;
	size_t InternalIndex = -1;
	Referencer* ReferencePtr = nullptr;

public:
	SObject();
	SObject(SObject&& Rhs) noexcept;
	virtual ~SObject() noexcept;

	void AddToRoot();
	void RemoveFromRoot();

private:
	SObject(const SObject&) = delete;

public:
	virtual std::wstring ToString();

protected:
	virtual void PostConstruction();

public:
	void* operator new(size_t);
	void operator delete(void*);
};