// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

template<class TBase>
class SubclassOf
{
	template<class>
	friend class SubclassOf;

	const Type* _Type = nullptr;

public:
	inline SubclassOf()
	{
	}

	inline SubclassOf(const SubclassOf& rhs) : _Type(rhs._Type)
	{
	}

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

	inline size_t GetHashCode() const
	{
		return _Type ? _Type->GetHashCode() : 0;
	}

	inline bool IsValid() const
	{
		return _Type != nullptr;
	}

	inline const Type* GetType() const
	{
		return _Type;
	}

	inline TBase* Instantiate(SObject* InOuter) const
	{
		if (!IsValid())
		{
			return nullptr;
		}
		return dynamic_cast<TBase*>(_Type->Instantiate(InOuter));
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