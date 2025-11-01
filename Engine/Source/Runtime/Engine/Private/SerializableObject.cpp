// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "SerializableObject.h"

namespace Ayla
{
	SerializableObject::SerializableObject()
	{
	}

	SerializableObject::~SerializableObject() noexcept
	{
	}

	void SerializableObject::MarkDirty()
	{
	}

	void SerializableObject::SetName(String name)
	{
		m_Name = name;
	}

	String SerializableObject::GetName() const
	{
		return m_Name;
	}

	void SerializableObject::Destroy(SharedPtr<SerializableObject> obj)
	{
		check(obj);
		if (ensure(obj->m_Disposed == false))
		{
			obj->m_Disposed = true;
		}
	}
}