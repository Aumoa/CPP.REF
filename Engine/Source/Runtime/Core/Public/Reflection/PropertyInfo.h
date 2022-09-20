// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "Concepts/IDerivedFrom.h"
#include "PropertyInfo.gen.h"

class Type;

SCLASS()
class CORE_API PropertyInfo : virtual public Object
{
	GENERATED_BODY()
	friend class Type;

private:
	libty::reflect::property_info_t _prop;

public:
	PropertyInfo(libty::reflect::property_info_t prop);

	String GetName() noexcept;
	void* GetValue(void* self);
	void SetValue(void* self, void* value);
	Type* GetPropertyType() noexcept;
	bool IsPublic() noexcept;
	bool IsProtected() noexcept;
	bool IsPrivate() noexcept;

	inline void* GetValue(Object* self)
	{
		return GetValue(reinterpret_cast<void*>(self));
	}

	inline void SetValue(Object* self, void* value)
	{
		SetValue(reinterpret_cast<void*>(self), value);
	}

	template<IDerivedFrom<Object> T, class TSelf>
	T* GetValue(TSelf&& self)
	{
		using TSelfNonRef = std::remove_reference_t<TSelf>;

		void* vself;
		if constexpr (std::is_pointer_v<TSelfNonRef> && IDerivedFrom<std::remove_pointer_t<TSelfNonRef>, Object>)
		{
			vself = reinterpret_cast<void*>(dynamic_cast<Object*>(self));
		}
		else
		{
			vself = &self;
		}

		void* result = GetValue(vself);
		return dynamic_cast<T*>(reinterpret_cast<Object*>(result));
	}

	template<IDerivedFrom<Object> T, class TSelf>
	void SetValue(TSelf&& self, T* value)
	{
		using TSelfNonRef = std::remove_reference_t<TSelf>;

		void* vself;
		if constexpr (std::is_pointer_v<TSelfNonRef> && IDerivedFrom<std::remove_pointer_t<TSelfNonRef>, Object>)
		{
			vself = reinterpret_cast<void*>(dynamic_cast<Object*>(self));
		}
		else
		{
			vself = &self;
		}

		SetValue(vself, dynamic_cast<Object*>(value));
	}

	template<class T, class TSelf>
	T& GetValue(TSelf&& self)
	{
		using TSelfNonRef = std::remove_reference_t<TSelf>;

		void* vself;
		if constexpr (std::is_pointer_v<TSelfNonRef> && IDerivedFrom<std::remove_pointer_t<TSelfNonRef>, Object>)
		{
			vself = reinterpret_cast<void*>(dynamic_cast<Object*>(self));
		}
		else
		{
			vself = &self;
		}

		void* result = GetValue(vself);
		return *reinterpret_cast<T*>(result);
	}

	template<class T, class TSelf>
	void SetValue(TSelf&& self, const T& value)
	{
		using TSelfNonRef = std::remove_reference_t<TSelf>;

		void* vself;
		if constexpr (std::is_pointer_v<TSelfNonRef> && IDerivedFrom<std::remove_pointer_t<TSelfNonRef>, Object>)
		{
			vself = reinterpret_cast<void*>(dynamic_cast<Object*>(self));
		}
		else
		{
			vself = &self;
		}

		SetValue(vself, const_cast<void*>(reinterpret_cast<const void*>(&value)));
	}
};