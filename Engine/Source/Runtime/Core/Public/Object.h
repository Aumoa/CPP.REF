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
#include "Threading/Spinlock.h"
#include <vector>
#include <functional>
#include <typeinfo>
#include <memory>

extern "C"
{
	PLATFORM_SHARED_EXPORT ::Ayla::ssize_t Ayla__Object__BeginWriteGCHandle__Injected(void* self);
	PLATFORM_SHARED_EXPORT void Ayla__Object__EndWriteGCHandle__Injected(void* self, ::Ayla::ssize_t handle, bool releaseIntPtr);
	PLATFORM_SHARED_EXPORT ::Ayla::ManagedTypeWrapper Ayla__Object__GetManagedType__Injected();
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
		friend ::Ayla::ssize_t (::Ayla__Object__BeginWriteGCHandle__Injected)(void* self);
		friend void ::Ayla__Object__EndWriteGCHandle__Injected(void* self, ssize_t handle, bool releaseIntPtr);
		friend ::Ayla::ManagedTypeWrapper (::Ayla__Object__GetManagedType__Injected)();

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

		Spinlock m_Spinlock;
		Type* m_Type;
		CreationFlags m_Flags;
		int32 m_Refs = 0;
		ssize_t m_GCHandle = 0;

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

		String ToString();
		Type* GetType() const { return m_Type; }

		void AddRef();
		void ReleaseRef();
		void* BindGCHandle__Unsafe(ssize_t gcHandlePtr);
		ObjectReferenceWrapper AsWrapper();
		
		template<class T>
		auto AsShared(this T&& self)
		{
			using U = std::remove_const_t<std::remove_reference_t<T>>;
			auto& hack = const_cast<U&>(self);
			hack.AddRef();
			return SharedPtr<U>(&hack);
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
				ptr.emplace((new T(std::forward<TArgs>(args)...))->AsShared());
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
				throw InvalidOperationException(TEXT("The constructor is not constructible."));
			}
		}

	private:
		static void ConfigureNew(const std::type_info& typeInfo, CreationFlags flags, std::function<void()> action);
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