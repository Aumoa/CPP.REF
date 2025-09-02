// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "NamedObject.h"

namespace Ayla
{
	NamedObject::NamedObject()
	{
	}

	NamedObject::~NamedObject() noexcept
	{
	}

	void NamedObject::SetName(String name)
	{
		if (m_Name != name)
		{
			m_Name = name;
			MarkDirty();
		}
	}
}