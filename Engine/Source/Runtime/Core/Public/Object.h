// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "InvalidOperationException.h"
#include "MemberAccessException.h"
#include "AssertionMacros.h"
#include "LanguageSupportMacros.h"
#include "Referencer.h"
#include "SharedPtr.h"
#include "Platform/PlatformMacros.h"
#include "Reflection/PropertyCollector.h"
#include "Reflection/ReflectionMacros.h"
#include "Marshal/ObjectReferenceWrapper.h"
#include "Marshal/ManagedTypeWrapper.h"
#include "Marshal/NativeExceptionInterop.h"
#include "Threading/Spinlock.h"
#include <vector>
#include <functional>
#include <typeinfo>
#include <memory>

extern "C"
{
	PLATFORM_SHARED_EXPORT ::Ayla::NativeCallStatus Ayla__Object__BeginWriteGCHandle__Injected(void* self, ::Ayla::ssize_t* handle) noexcept;
	PLATFORM_SHARED_EXPORT ::Ayla::NativeCallStatus Ayla__Object__EndWriteGCHandle__Injected(void* self, ::Ayla::ssize_t handle, bool releaseIntPtr, ::Ayla::uint64* gcHandleSerial) noexcept;
	PLATFORM_SHARED_EXPORT ::Ayla::NativeCallStatus Ayla__Object__ClearGCHandle__Injected(void* self, ::Ayla::uint64 gcHandleSerial, ::Ayla::ssize_t* handle) noexcept;
	PLATFORM_SHARED_EXPORT ::Ayla::NativeCallStatus Ayla__Object__GetManagedType__Injected(::Ayla::ManagedTypeWrapper* result) noexcept;
}

namespace Ayla
{
	struct TypeRegister;
	class Type;
	class RuntimeType;

	ACLASS()
	class CORE_API Object
	{
		friend TypeRegister;
		friend Type;
		friend RuntimeType;
		friend ::Ayla::NativeCallStatus (::Ayla__Object__BeginWriteGCHandle__Injected)(void* self, ::Ayla::ssize_t* handle) noexcept;
		friend ::Ayla::NativeCallStatus (::Ayla__Object__EndWriteGCHandle__Injected)(void* self, ::Ayla::ssize_t handle, bool releaseIntPtr, ::Ayla::uint64* gcHandleSerial) noexcept;
		friend ::Ayla::NativeCallStatus (::Ayla__Object__ClearGCHandle__Injected)(void* self, ::Ayla::uint64 gcHandleSerial, ::Ayla::ssize_t* handle) noexcept;
		friend ::Ayla::NativeCallStatus (::Ayla__Object__GetManagedType__Injected)(::Ayla::ManagedTypeWrapper* result) noexcept;

	public:
		using This = Object;

	private:
		struct CreationHack;

	public:
		enum class CreationFlags : uint8
		{
			None,
			FromScript = 1 << 0
		};

		GENERATE_BITMASK_ENUM_OPERATORS_FRIEND(::Ayla::Object::CreationFlags);

	public:
		static ManagedTypeWrapper GetManagedType();

	private:
		static size_t s_LiveObjects;
		static volatile uint64 s_NextGCHandleSerial;

		Spinlock m_Spinlock;
		Type* m_Type;
		CreationFlags m_Flags;
		int32 m_Refs = 0;
		ssize_t m_GCHandle = 0;
		uint64 m_GCHandleSerial = 0;

	protected:
		static void GatherProperties(PropertyCollector& collection)
		{
			PLATFORM_UNREFERENCED_PARAMETER(collection);
		}

	public:
		Object();
		Object(const Object&) = delete;
		Object(Object&&) = delete;
		virtual ~Object() noexcept;

		String ToString() const;
		Type* GetType() const { return m_Type; }

		void AddRef();
		void ReleaseRef();
		BoundObjectReferenceWrapper BindGCHandle__Unsafe(ssize_t gcHandlePtr);
		ObjectReferenceWrapper AsWrapper();

		template<std::derived_from<Object> T = Object>
		auto AsShared()
		{
			auto* self = static_cast<T*>(this);
			self->AddRef();
			return SharedPtr<T>(self);
		}

		template<std::derived_from<Object> T = Object>
		auto AsShared() const
		{
			auto* self = const_cast<T*>(static_cast<const T*>(this));
			self->AddRef();
			return SharedPtr<T>(self);
		}

		Object& operator =(const Object&) = delete;
		Object& operator =(Object&&) = delete;

	public:
		template<std::derived_from<Object> T, class... TArgs>
		static SharedPtr<T> New(TArgs&&... args)
		{
			std::optional<SharedPtr<T>> ptr;
			ConfigureNew(typeid(T), CreationFlags::None, [&]()
			{
				ptr.emplace((new T(std::forward<TArgs>(args)...))->template AsShared<T>());
			});
			return std::move(ptr).value();
		}

		template<std::derived_from<Object> T, class... TArgs>
		static SharedPtr<T> UnsafeNew(TArgs&&... args)
		{
			if constexpr (std::is_constructible_v<T, TArgs...>)
			{
				return New<T>(std::forward<TArgs>(args)...);
			}
			else
			{
				throw InvalidOperationException(TEXT("The constructor is not constructible."));
			}
		}

		template<std::derived_from<Object> T, class... TArgs>
		static T* ScriptNew(TArgs&&... args)
		{
			if constexpr (std::is_constructible_v<T, TArgs...>)
			{
				T* ptr;
				ConfigureNew(typeid(T), CreationFlags::FromScript, [&]()
				{
					ptr = new T(std::forward<TArgs>(args)...);
				});
				return ptr;
			}
			else
			{
				failMsgf(TEXT("The construtor is not constructible."));
			}
		}

	private:
		static void ConfigureNew(const std::type_info& typeInfo, CreationFlags flags, std::function<void()> action);
		uint64 SetGCHandle__Unsafe(ssize_t gcHandlePtr);
		ObjectReferenceWrapper AsWrapper__Unsafe();
		BoundObjectReferenceWrapper AsBoundWrapper__Unsafe();
	};

	template<class T>
	struct is_shared_object : public std::false_type
	{
	};

	template<std::derived_from<Object> T>
	struct is_shared_object<std::shared_ptr<T>> : public std::true_type
	{
	};

	template<class T>
	concept object_reference = is_shared_object<T>::value;
}
