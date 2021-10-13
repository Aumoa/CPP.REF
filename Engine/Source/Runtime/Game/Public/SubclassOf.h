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

	const Type* _Type = nullptr;

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
	inline SubclassOf(const SubclassOf& rhs) : _Type(rhs._Type)
	{
	}

	/// <summary>
	/// Initialize new <see cref="SubclassOf"/> instance.
	/// </summary>
	template<std::derived_from<TBase> TOther>
	inline SubclassOf(const SubclassOf<TOther>& rhs) : _Type(rhs._Type)
	{
	}

	inline SubclassOf(const Type* type) : _Type(type)
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
		return _Type ? _Type->GetHashCode() : 0;
	}

	/// <summary>
	/// Represents this is valid state.
	/// </summary>
	inline bool IsValid() const
	{
		return _Type != nullptr;
	}

	inline const Type* GetType() const
	{
		return _Type;
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
		return dynamic_cast<TBase*>(_Type->Instantiate());
	}

	inline SubclassOf& operator =(const SubclassOf& rhs)
	{
		_Type = rhs._Type;
		return *this;
	}

	template<std::derived_from<TBase> TOther>
	inline SubclassOf& operator =(const SubclassOf<TOther>& rhs)
	{
		_Type = rhs._Type;
		return *this;
	}

	inline bool operator ==(const SubclassOf& rhs) const
	{
		if (_Type == nullptr)
		{
			return rhs._Type != nullptr;
		}
		else if (rhs._Type == nullptr)
		{
			return _Type != nullptr;
		}
		else
		{
			return _Type->IsA(rhs._Type);
		}
	}

	inline bool operator !=(const SubclassOf& rhs) const
	{
		return !operator ==(rhs);
	}
};