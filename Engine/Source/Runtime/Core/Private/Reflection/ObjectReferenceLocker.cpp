// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Reflection/ObjectReferenceLocker.h"

extern "C"
{
	PLATFORM_SHARED_EXPORT ::Ayla::Object* Ayla__ObjectReferenceLocker__GetRawPointer__Injected(::Ayla::ObjectReferenceLocker* self)
	{
		return reinterpret_cast<::std::shared_ptr<::Ayla::Object>*>(self->Ref)->get();
	}

	PLATFORM_SHARED_EXPORT void Ayla__ObjectReferenceLocker__Destroy__Injected(::Ayla::ObjectReferenceLocker* self)
	{
		auto ref = reinterpret_cast<::std::shared_ptr<::Ayla::Object>*>(self->Ref);
		delete ref;
		self->Ref = 0;
	}
}