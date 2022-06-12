// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "Type.generated.h"

SCLASS()
class CORE_API Type : virtual public Object
{
	GENERATED_BODY()
	friend class Object;

private:
	String _name;

private:
	Type() noexcept;

	template<class T>
	static Type* Generate() noexcept
	{
		static Type sType;
		return &sType;
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