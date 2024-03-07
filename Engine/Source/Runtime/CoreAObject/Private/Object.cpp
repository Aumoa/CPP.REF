// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Object.h"
#include "ObjectReference.h"

AObject::AObject()
	: Referencer(new ObjectReference(this))
{
}

AObject::~AObject() noexcept
{
}

extern "C"
{
	PLATFORM_SHARED_EXPORT AObject* AObject__Constructor()
	{
		return new AObject();
	}

	PLATFORM_SHARED_EXPORT void AObject__Destroy(ObjectReference* reference)
	{
		if (reference == nullptr)
		{
			return;
		}

		ObjectReference::Destroy(*reference);
	}
}