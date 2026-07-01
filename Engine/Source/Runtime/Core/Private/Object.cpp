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
#include "Marshal/ManagedCallBoundary.h"
#include "Marshal/NativeCallBoundary.h"
#include "Marshal/ManagedStringWrapper.h"
#include <mutex>

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
	volatile uint64 Object::s_NextGCHandleSerial;
	CoreCLRFunctions g_CoreCLRFunctions;

	void EnsureCoreCLRFunctionsInitialized()
	{
		static std::once_flag s_InitializeCoreCLRFunctionsOnce;
		std::call_once(s_InitializeCoreCLRFunctionsOnce, []()
		{
			using signature_t = NativeCallStatus(*)(CoreCLRFunctions*);
			auto function = (signature_t)ScriptingBackend::Get().GetFunctionPointer("Core.Script", "Ayla.CoreCLRFunctions", "Get__Invoke");
			ManagedCallBoundary::ThrowIfFailed(function(&g_CoreCLRFunctions));
		});
	}

	ManagedTypeWrapper Object::GetManagedType()
	{
		static ManagedTypeWrapper s_Type = []()
		{
			using signature_t = NativeCallStatus(*)(void**);
			auto function = reinterpret_cast<signature_t>(ScriptingBackend::Get().GetFunctionPointer("Core.Script", "Ayla.Object", "GetScriptType__Invoke"));
			void* scriptTypeGetter = nullptr;
			ManagedCallBoundary::ThrowIfFailed(function(&scriptTypeGetter));

			return ManagedTypeWrapper
			{
				.NativeType = TypeCollector::FindType(typeid(Object)),
				.ScriptTypeGetter = scriptTypeGetter
			};
		}();

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

		EnsureCoreCLRFunctionsInitialized();

		PlatformAtomics::InterlockedIncrement(&s_LiveObjects);
	}

	Object::~Object() noexcept
	{
		PlatformAtomics::InterlockedDecrement(&s_LiveObjects);
	}

	String Object::ToString() const
	{
		return String::Format(TEXT("{}"), String::FromLiteral(typeid(*this).name()));
	}

	void Object::AddRef()
	{
		auto lock = std::unique_lock{ m_Spinlock };
		if (m_Refs++ == 0 && m_GCHandle != 0)
		{
			ManagedCallBoundary::ThrowIfFailed(g_CoreCLRFunctions.m_AsHardHandle__Invoke(&m_GCHandle));
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
				ManagedCallBoundary::ThrowIfFailed(g_CoreCLRFunctions.m_AsWeakHandle__Invoke(&m_GCHandle));
			}
		}
	}

	BoundObjectReferenceWrapper Object::BindGCHandle__Unsafe(ssize_t gcHandlePtr)
	{
		auto lock = std::unique_lock{ m_Spinlock };
		SetGCHandle__Unsafe(gcHandlePtr);
		return AsBoundWrapper__Unsafe();
	}

	NativeObjectReferenceWrapper Object::AsNativeObjectReferenceWrapper()
	{
		auto lock = std::unique_lock{ m_Spinlock };
		return AsNativeObjectReferenceWrapper__Unsafe();
	}

	uint64 Object::SetGCHandle__Unsafe(ssize_t gcHandlePtr)
	{
		if (m_GCHandle == gcHandlePtr && (gcHandlePtr == 0 || m_GCHandleSerial != 0))
		{
			return m_GCHandleSerial;
		}

		m_GCHandle = gcHandlePtr;
		m_GCHandleSerial = gcHandlePtr != 0
			? PlatformAtomics::InterlockedIncrement(&s_NextGCHandleSerial)
			: 0;
		return m_GCHandleSerial;
	}

	NativeObjectReferenceWrapper Object::AsNativeObjectReferenceWrapper__Unsafe()
	{
		return NativeObjectReferenceWrapper
		{
			.Ptr = reinterpret_cast<ssize_t>(this),
			.IntGCHandlePtr = 0,
			.GCHandleSerial = m_GCHandleSerial
		};
	}

	BoundObjectReferenceWrapper Object::AsBoundWrapper__Unsafe()
	{
		return BoundObjectReferenceWrapper
		{
			.Ptr = reinterpret_cast<ssize_t>(this),
			.GCHandleSerial = m_GCHandleSerial
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
	PLATFORM_SHARED_EXPORT ::Ayla::NativeCallStatus Ayla__Object__BeginWriteGCHandle__Injected(void* self, ::Ayla::ssize_t* handle) noexcept
	{
		return ::Ayla::NativeCallBoundary::Invoke([&]() -> ::Ayla::NativeCallStatus
		{
			auto self_ = (::Ayla::Object*)self;
			self_->m_Spinlock.lock();
			*handle = self_->m_GCHandle;
			return ::Ayla::NativeCallStatus::Succeeded;
		});
	}

	PLATFORM_SHARED_EXPORT ::Ayla::NativeCallStatus Ayla__Object__EndWriteGCHandle__Injected(void* self, ::Ayla::ssize_t handle, bool releaseIntPtr, ::Ayla::uint64* gcHandleSerial) noexcept
	{
		return ::Ayla::NativeCallBoundary::Invoke([&]() -> ::Ayla::NativeCallStatus
		{
			auto self_ = (::Ayla::Object*)self;
			auto lock = std::unique_lock{ self_->m_Spinlock, std::adopt_lock };
			auto serial = self_->SetGCHandle__Unsafe(handle);
			if (gcHandleSerial != nullptr)
			{
				*gcHandleSerial = serial;
			}

			if (releaseIntPtr)
			{
				--self_->m_Refs;
				check(self_->m_Refs >= 0);
			}
			if (releaseIntPtr && self_->m_Refs == 0)
			{
				if (handle == 0)
				{
					lock.unlock();
					delete self_;
					return ::Ayla::NativeCallStatus::Succeeded;
				}

				::Ayla::ManagedCallBoundary::ThrowIfFailed(::Ayla::g_CoreCLRFunctions.m_AsWeakHandle__Invoke(&self_->m_GCHandle));
			}
			lock.unlock();
			return ::Ayla::NativeCallStatus::Succeeded;
		});
	}

	PLATFORM_SHARED_EXPORT ::Ayla::NativeCallStatus Ayla__Object__ClearGCHandle__Injected(void* self, ::Ayla::uint64 gcHandleSerial, ::Ayla::ssize_t* handle) noexcept
	{
		return ::Ayla::NativeCallBoundary::Invoke([&]() -> ::Ayla::NativeCallStatus
		{
			auto self_ = (::Ayla::Object*)self;
			self_->m_Spinlock.lock();
			if (gcHandleSerial == 0 || self_->m_GCHandleSerial != gcHandleSerial)
			{
				*handle = 0;
				self_->m_Spinlock.unlock();
				return ::Ayla::NativeCallStatus::Succeeded;
			}

			auto gcHandle = self_->m_GCHandle;
			self_->SetGCHandle__Unsafe(0);
			*handle = gcHandle;
			if (self_->m_Refs == 0)
			{
				self_->m_Spinlock.unlock();
				delete self_;
				return ::Ayla::NativeCallStatus::Succeeded;
			}

			self_->m_Spinlock.unlock();
			return ::Ayla::NativeCallStatus::Succeeded;
		});
	}

	PLATFORM_SHARED_EXPORT ::Ayla::NativeCallStatus Ayla__Object__GetManagedType__Injected(::Ayla::ManagedTypeWrapper* result) noexcept
	{
		return ::Ayla::NativeCallBoundary::Invoke([&]() -> ::Ayla::NativeCallStatus
		{
			*result = ::Ayla::Object::GetManagedType();
			return ::Ayla::NativeCallStatus::Succeeded;
		});
	}

	PLATFORM_SHARED_EXPORT ::Ayla::NativeCallStatus Ayla__Object__GetManagedTypeFromPtr__Injected(::Ayla::Object* self, ::Ayla::ManagedTypeWrapper* result) noexcept
	{
		return ::Ayla::NativeCallBoundary::Invoke([&]() -> ::Ayla::NativeCallStatus
		{
			*result = self->GetType()->GetManagedType();
			return ::Ayla::NativeCallStatus::Succeeded;
		});
	}
}
