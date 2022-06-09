// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"

SCLASS()
class CORE_API Type : virtual public Object
{
	GENERATED_BODY()
	friend class Object;

private:
	String _name;
	String _fullname;

private:
	Type() noexcept;

	template<class T>
	static Type* Generate() noexcept
	{
		return nullptr;
	}

public:
	virtual ~Type() noexcept override;

	virtual String ToString() const noexcept override;

	String GetName() const noexcept;
};

template<class TSelf>
Type* Object::GetType(this TSelf&&) noexcept
{
	return Type::Generate<TSelf>();
}