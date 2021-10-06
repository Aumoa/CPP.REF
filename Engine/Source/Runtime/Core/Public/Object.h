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
	SObject* _outer = nullptr;
	std::vector<std::shared_ptr<SObject>> _subobjects;
	std::map<SObject*, size_t> _views;

public:
	/// <summary>
	/// Initialize new <see cref="SObject"/> class instance.
	/// </summary>
	SObject();
	virtual ~SObject() noexcept;

private:
	SObject(const SObject&) = delete;
	SObject(SObject&&) = delete;

public:
	/// <summary>
	/// Get a simple string representing this object.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The string view instance. </returns>
	virtual std::wstring ToString(std::wstring_view formatArgs = L"");
	SFUNCTION(ToString);

	/// <summary>
	/// Create object which linked outer to this.
	/// </summary>
	/// <typeparam name="T"> Type of subobject. </typeparam>
	/// <typeparam name="...TArgs"> The type sequence of constructor arguments. </typeparam>
	/// <param name="...args"> The constructor arguments. </param>
	/// <returns> The instantiated pointer. </returns>
	template<class T, class... TArgs> requires std::constructible_from<T, TArgs...>
	T* NewObject(TArgs&&... args)
	{
		std::shared_ptr shared = std::make_shared<T>(std::forward<TArgs>(args)...);
		auto ptr = shared.get();
		InternalAttachSubobject(ptr);
		ptr->_outer = this;
		return ptr;
	}

	/// <summary>
	/// Create object without outer.
	/// </summary>
	/// <typeparam name="T"> Type of subobject. </typeparam>
	/// <typeparam name="...TArgs"> The type sequence of constructor arguments. </typeparam>
	/// <param name="...args"> The constructor arguments. </param>
	/// <returns> The instantiated pointer. </returns>
	template<class T, class... TArgs> requires std::constructible_from<T, TArgs...>
	static std::shared_ptr<T> NewStaticObject(TArgs&&... args)
	{
		return std::make_shared<T>(std::forward<TArgs>(args)...);
	}

	/// <summary>
	/// Get outer that owner of this object.
	/// </summary>
	/// <returns> The outer object. </returns>
	SObject* GetOuter() const;
	SFUNCTION(GetOuter);

	/// <summary>
	/// Change outer. SObject will destroy when outer be destroyed.
	/// </summary>
	/// <param name="newOuter"> The new outer. </param>
	/// <returns> The object pointer if attachment is failed. </returns>
	std::shared_ptr<SObject> SetOuter(SObject* newOuter);
	SFUNCTION(SetOuter);

	/// <summary>
	/// Destroy subobject.
	/// </summary>
	/// <param name="subobject"> The target object. </param>
	static void DestroySubobject(SObject* subobject);

	template<class T> requires std::derived_from<T, SObject>
	T* As() { return dynamic_cast<T*>(this); }
	template<class T> requires std::derived_from<T, SObject>
	const T* As() const { return dynamic_cast<const T*>(this); }

	/// <summary>
	/// Casts between two SObject classes.
	/// </summary>
	template<std::derived_from<SObject> TTo, std::derived_from<SObject> TFrom>
	inline static TTo* Cast(TFrom* from)
	{
		if (from)
		{
			return from->template As<TTo>();
		}
		return nullptr;
	}

	/// <summary>
	/// Casts between two SObject classes.
	/// </summary>
	template<std::derived_from<SObject> TTo, std::derived_from<SObject> TFrom>
	inline static const TTo* Cast(const TFrom* from)
	{
		if (from)
		{
			return from->template As<TTo>();
		}
	}

	/// <summary>
	/// Casts from native type to boxing object. This cast require instanced SObject for setting outer.
	/// </summary>
	template<std::same_as<SObject> TTo, class TFrom>
	inline TTo* Cast(const TFrom& value) requires (!std::derived_from<TFrom, SObject>)
	{
		return NewObject<SValueType>(value);
	}

	/// <summary>
	/// Casts from boxing object to native type.
	/// </summary>
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