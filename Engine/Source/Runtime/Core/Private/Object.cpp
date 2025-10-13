// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Type.h"
#include "TypeNotFoundException.h"
#include "try_finally.h"
#include "Platform/PlatformAtomics.h"
#include "Reflection/TypeRegister.h"
#include "Reflection/TypeCollector.h"
#include "Reflection/ReflectionMacros.h"
#include "ScriptingBackend/ScriptingBackend.h"

ACLASS__IMPL_CLASS_REGISTER(Ayla, Object);

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

	ManagedTypeWrapper Object::GetManagedType()
	{
		using signature_t = void*(*)();

		static ManagedTypeWrapper s_Type =
		{
			.NativeType = TypeCollector::FindType(typeid(Object)),
			.ScriptTypeGetter = reinterpret_cast<signature_t>(ScriptingBackend::Get().GetFunctionPointer("Core.Script", "Ayla.Object", "GetScriptType__Invoke"))()
		};

		return s_Type;
	}

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
			.Flags = (int32)m_Flags
		};
	}

	ObjectReferenceWrapper Object::AsWrapper()
	{
		auto lock = std::unique_lock{ m_Spinlock };
		return ObjectReferenceWrapper
		{
			.IntRef = m_GCHandle == 0 ? (ssize_t)new std::shared_ptr<Object>(shared_from_this()) : 0,
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
	PLATFORM_SHARED_EXPORT void Ayla__Object__DeleteIntermediateRef__Injected(void* self)
	{
		delete reinterpret_cast<std::shared_ptr<::Ayla::Object>*>(self);
	}

	PLATFORM_SHARED_EXPORT ::Ayla::ssize_t Ayla__Object__BeginWriteGCHandle__Injected(void* self)
	{
		auto self_ = (::Ayla::Object*)self;
		self_->m_Spinlock.lock();
		return self_->m_GCHandle;
	}

	PLATFORM_SHARED_EXPORT void Ayla__Object__EndWriteGCHandle__Injected(void* self, ::Ayla::ssize_t handle)
	{
		auto self_ = (::Ayla::Object*)self;
		self_->m_GCHandle = handle;
		self_->m_Spinlock.unlock();
	}

	PLATFORM_SHARED_EXPORT::Ayla::ManagedTypeWrapper Ayla__Object__GetManagedType__Injected()
	{
		return ::Ayla::Object::GetManagedType();
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