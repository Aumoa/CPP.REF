// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

/// <summary>
/// Represents subclass of base type.
/// </summary>
/// <typeparam name="TBase"> The base class. </typeparam>
template<class TBase>
class SubclassOf
{
	template<class>
	friend class SubclassOf;

	const Type* _type = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="SubclassOf"/> instance.
	/// </summary>
	inline SubclassOf()
	{
	}

	/// <summary>
	/// Initialize new <see cref="SubclassOf"/> instance.
	/// </summary>
	inline SubclassOf(const SubclassOf& rhs) : _type(rhs._type)
	{
	}

	/// <summary>
	/// Initialize new <see cref="SubclassOf"/> instance.
	/// </summary>
	template<std::derived_from<TBase> TOther>
	inline SubclassOf(const SubclassOf<TOther>& rhs) : _type(rhs._type)
	{
	}

	inline SubclassOf(const Type* type) : _type(type)
	{
		if (type != nullptr)
		{
			check(type->IsDerivedFrom<TBase>());
		}
	}

	/// <summary>
	/// Get identifier hash code.
	/// </summary>
	inline size_t GetHashCode() const
	{
		return _type ? _type->GetHashCode() : 0;
	}

	/// <summary>
	/// Represents this is valid state.
	/// </summary>
	inline bool IsValid() const
	{
		return _type != nullptr;
	}

	/// <summary>
	/// Instantiate saved class as base class.
	/// </summary>
	/// <param name="outer"> Outer object of instance. </param>
	inline TBase* Instantiate(SObject* outer) const
	{
		if (!IsValid())
		{
			return nullptr;
		}
		return dynamic_cast<TBase*>(_type->Instantiate());
	}

	inline SubclassOf& operator =(const SubclassOf& rhs)
	{
		_type = rhs._type;
		return *this;
	}

	template<std::derived_from<TBase> TOther>
	inline SubclassOf& operator =(const SubclassOf<TOther>& rhs)
	{
		_type = rhs._type;
		return *this;
	}

	inline bool operator ==(const SubclassOf& rhs) const
	{
		if (_type == nullptr)
		{
			return rhs._type != nullptr;
		}
		else if (rhs._type == nullptr)
		{
			return _type != nullptr;
		}
		else
		{
			return _type->IsA(rhs._type);
		}
	}

	inline bool operator !=(const SubclassOf& rhs) const
	{
		return !operator ==(rhs);
	}
};