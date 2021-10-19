// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <set>
#include <string>
#include <string_view>
#include <functional>
#include <map>
#include "PrimitiveTypes.h"
#include "LogCore.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "Reflection/ReflectionMacros.h"

class SValueType;

/// <summary>
/// Supports all classes in the smart component hierarchy and provides low-level services to derived classes.
/// Represents unit that subobjects are binding and managed.
/// </summary>
class CORE_API SObject : public std::enable_shared_from_this<SObject>
{
	GENERATED_BODY(SObject, virtual)

private:
	static std::atomic<uint64> _InternalObjectIndexGenerator;
	const uint64 _InternalObjectIndex;
	SObject* _outer = nullptr;
	std::vector<std::shared_ptr<SObject>> _subobjects;
	std::map<SObject*, size_t> _views;

public:
	SObject();
	virtual ~SObject() noexcept;

private:
	SObject(const SObject&) = delete;
	SObject(SObject&&) = delete;

public:
	virtual std::wstring ToString(std::wstring_view formatArgs = L"");

	template<class T, class... TArgs> requires std::constructible_from<T, TArgs...>
	T* NewObject(TArgs&&... args)
	{
		std::shared_ptr shared = std::make_shared<T>(std::forward<TArgs>(args)...);
		auto ptr = shared.get();
		InternalAttachSubobject(ptr);
		ptr->_outer = this;
		return ptr;
	}

	SObject* GetOuter() const;
	std::shared_ptr<SObject> SetOuter(SObject* newOuter);
	uint64 GetInternalIndex();

	static void DestroySubobject(SObject* subobject);

	template<std::derived_from<SObject> TTo, std::derived_from<SObject> TFrom>
	inline static TTo* Cast(TFrom* from)
	{
		return dynamic_cast<TTo*>(from);
	}

	template<std::derived_from<SObject> TTo, std::derived_from<SObject> TFrom>
	inline static const TTo* Cast(const TFrom* from)
	{
		return dynamic_cast<const TTo*>(from);
	}

	template<std::same_as<SObject> TTo, class TFrom>
	inline TTo* Cast(const TFrom& value) requires (!std::derived_from<TFrom, SObject>)
	{
		return NewObject<SValueType>(value);
	}

	template<class TTo, std::same_as<SObject> TFrom>
	static inline TTo Cast(const TFrom* value) requires (!std::derived_from<TTo, SObject>)
	{
		auto boxing = Cast<SValueType>(value);
		if (boxing == nullptr)
		{
			SE_LOG(LogCasts, Fatal, L"Object is not boxing class.");
		}

		TTo value;
		if (!boxing->Unboxing(&value))
		{
			SE_LOG(LogCasts, Fatal, L"The type of value contained at boxing object is not match with desired type.");
		}

		return value;
	}

	void* operator new(size_t);
	void operator delete(void*);

private:
	void InternalDetachSubobject(SObject* subobject);
	void InternalAttachSubobject(SObject* subobject);
};

#define implements virtual public 
#include "ValueType.h"