// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Type.h"
#include "TypeNotFoundException.h"
#include "try_finally.h"
#include "Platform/PlatformAtomics.h"
#include "Reflection/TypeRegister.h"
#include "Reflection/TypeCollector.h"
#include "Reflection/ReflectionMacros.h"
#include "Reflection/Marshal.h"

namespace Ayla
{
	struct Object::CreationHack
	{
		static thread_local CreationHack s_Hack;
		bool AllowConstruct = false;
		Type* ObjectType;
		CreationFlags Flags;

		static inline CreationHack& Begin(Type* type, CreationFlags flags)
		{
			s_Hack.AllowConstruct = true;
			s_Hack.ObjectType = type;
			s_Hack.Flags = flags;
			return s_Hack;
		}

		inline void End()
		{
			AllowConstruct = false;
			ObjectType = nullptr;
			Flags = {};
		}
	};

	thread_local Object::CreationHack Object::CreationHack::s_Hack;
	size_t Object::s_LiveObjects;
	Spinlock Object::s_Spinlock;

	Object::Object()
		: m_Type{ CreationHack::s_Hack.ObjectType }
		, m_Flags{ CreationHack::s_Hack.Flags }
	{
		if (CreationHack::s_Hack.AllowConstruct == false)
		{
			throw InvalidOperationException(TEXT("Object must be created with Ayla::Object::New<T> function."));
		}

		PlatformAtomics::InterlockedIncrement(&s_LiveObjects);
	}

	Object::~Object() noexcept
	{
		PlatformAtomics::InterlockedDecrement(&s_LiveObjects);
	}

	String Object::ToString()
	{
		return String::Format(TEXT("{}"), String::FromLiteral(typeid(*this).name()));
	}

	ObjectReferenceLocker Object::CreateLocker()
	{
		return ObjectReferenceLocker
		{
			.Ref = reinterpret_cast<ssize_t>(new std::shared_ptr<Object>(shared_from_this())),
			.Flags =(int32)m_Flags
		};
	}

	ObjectReferenceWrapper Object::AsWrapper()
	{
		return ObjectReferenceWrapper
		{
			.Ptr = reinterpret_cast<ssize_t>(this),
			.Handle = m_GCHandle
		};
	}

	void Object::ConfigureNew(const std::type_info& typeInfo, CreationFlags flags, std::function<void()> action)
	{
		auto type = TypeCollector::FindType(typeInfo);
		if (type == nullptr)
		{
			throw new TypeNotFoundException(String::FromLiteral(typeInfo.name()));
		}

		auto& hack = CreationHack::Begin(type, flags);
		action();
		hack.End();
	}
}

extern "C"
{
	PLATFORM_SHARED_EXPORT ::Ayla::ssize_t Ayla__Object__BeginWriteGCHandle__Injected(void* self)
	{
		::Ayla::Object::s_Spinlock.lock();
		return ((::Ayla::Object*)self)->m_GCHandle;
	}

	PLATFORM_SHARED_EXPORT void Ayla__Object__EndWriteGCHandle__Injected(void* self, ::Ayla::ssize_t handle)
	{
		((::Ayla::Object*)self)->m_GCHandle = handle;
		::Ayla::Object::s_Spinlock.unlock();
	}

	PLATFORM_SHARED_EXPORT ::Ayla::ObjectReferenceWrapper Ayla__Object__AsWrapper__Injected(::Ayla::Object* self)
	{
		return self->AsWrapper();
	}

	PLATFORM_SHARED_EXPORT ::Ayla::ObjectReferenceLocker Ayla__Object__CreateLocker__Injected(::Ayla::Object* self)
	{
		return self->CreateLocker();
	}
}