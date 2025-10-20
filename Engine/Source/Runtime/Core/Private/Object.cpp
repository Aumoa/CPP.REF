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
#include "Marshal/CoreCLRFunctions.h"
#include "Marshal/ManagedStringWrapper.h"

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
	CoreCLRFunctions g_CoreCLRFunctions;

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

		static int s_StaticConstruct = []() -> int
		{
			using signature_t = CoreCLRFunctions(*)();
			auto function = (signature_t)ScriptingBackend::Get().GetFunctionPointer("Core.Script", "Ayla.CoreCLRFunctions", "Get__Invoke");
			g_CoreCLRFunctions = function();
			return 0;
		}();

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

	void Object::AddRef()
	{
		auto lock = std::unique_lock{ m_Spinlock };
		if (m_Refs++ == 0 && m_GCHandle != 0)
		{
			g_CoreCLRFunctions.AsHardHandle__Invoke(&m_GCHandle);
		}
	}

	void Object::ReleaseRef()
	{
		auto lock = std::unique_lock{ m_Spinlock };
		if (--m_Refs == 0)
		{
			if (m_GCHandle == 0)
			{
				lock.unlock();
				delete this;
				return;
			}
			else
			{
				g_CoreCLRFunctions.AsWeakHandle__Invoke(&m_GCHandle);
			}
		}
	}

	void* Object::BindGCHandle__Unsafe(ssize_t gcHandlePtr)
	{
		m_GCHandle = gcHandlePtr;
		return this;
	}

	ObjectReferenceWrapper Object::AsWrapper()
	{
		return ObjectReferenceWrapper
		{
			.Ptr = reinterpret_cast<ssize_t>(this),
			.IntGCHandlePtr = 0
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
		auto self_ = (::Ayla::Object*)self;
		self_->m_Spinlock.lock();
		return self_->m_GCHandle;
	}

	PLATFORM_SHARED_EXPORT void Ayla__Object__EndWriteGCHandle__Injected(void* self, ::Ayla::ssize_t handle, bool releaseIntPtr)
	{
		auto self_ = (::Ayla::Object*)self;
		self_->m_GCHandle = handle;
		if (releaseIntPtr)
		{
			--self_->m_Refs;
			check(self_->m_Refs != 0 || self_->m_GCHandle);
		}
		if (self_->m_Refs == 0 && handle == 0)
		{
			self_->m_Spinlock.unlock();
			delete self_;
		}
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

	PLATFORM_SHARED_EXPORT ::Ayla::ManagedTypeWrapper Ayla__Object__GetManagedTypeFromPtr__Injected(::Ayla::Object* self)
	{
		return self->GetType()->GetManagedType();
	}
}