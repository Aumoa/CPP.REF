// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"

class CORE_API Type : virtual public Object
{
	friend class Object;

private:
	Type();

	template<class T>
	static Type* Generate()
	{
		return nullptr;
	}

public:
	virtual ~Type() noexcept override;

	String GetName() const noexcept;
};

template<class TSelf>
Type* Object::GetType(this const TSelf&)
{
	return Type::Generate<TSelf>();
}