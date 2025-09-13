// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "InvalidOperationException.h"
#include "AssertionMacros.h"
#include "LanguageSupportMacros.h"
#include "Referencer.h"
#include "Platform/PlatformMacros.h"
#include "Reflection/PropertyCollector.h"
#include "Reflection/ReflectionMacros.h"
#include "Reflection/ObjectReferenceWrapper.h"
#include "Reflection/ObjectReferenceLocker.h"
#include "Threading/Spinlock.h"
#include <vector>
#include <functional>
#include <typeinfo>
#include <memory>

extern "C"
{
	PLATFORM_SHARED_EXPORT ::Ayla::ssize_t Ayla__Object__BeginWriteGCHandle__Injected(void* self);
	PLATFORM_SHARED_EXPORT void Ayla__Object__EndWriteGCHandle__Injected(void* self, ::Ayla::ssize_t handle);
}

namespace Ayla
{
	struct TypeRegister;
	class Type;
	class RuntimeType;

	ACLASS()
	class CORE_API Object : public std::enable_shared_from_this<Object>
	{
		friend TypeRegister;
		friend Type;
		friend RuntimeType;
		friend ::Ayla::ssize_t (::Ayla__Object__BeginWriteGCHandle__Injected)(void* self);
		friend void ::Ayla__Object__EndWriteGCHandle__Injected(void* self, ssize_t handle);

	public:
		using This = Object;

	private:
		struct CreationHack;

	public:
		enum class CreationFlags
		{
			None,
			FromScript = 1 << 0
		};

		GENERATE_BITMASK_ENUM_OPERATORS_FRIEND(::Ayla::Object::CreationFlags);

	private:
		static size_t s_LiveObjects;

		Spinlock m_Spinlock;
		Type* m_Type;
		CreationFlags m_Flags;
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

		ObjectReferenceLocker CreateLocker();
		ObjectReferenceWrapper AsWrapper();

		Object& operator =(const Object&) = delete;
		Object& operator =(Object&&) = delete;

	public:
		template<std::derived_from<Object> T, class... TArgs>
		static std::shared_ptr<T> New(TArgs&&... args)
		{
			std::optional<std::shared_ptr<T>> ptr;
			ConfigureNew(typeid(T), CreationFlags::None, [&]()
			{
				ptr.emplace(std::make_shared<T>(std::forward<TArgs>(args)...));
			});
			return std::move(ptr).value();
		}

		template<std::derived_from<Object> T, class... TArgs>
		static std::shared_ptr<T> ScriptNew(TArgs&&... args)
		{
			std::optional<std::shared_ptr<T>> ptr;
			ConfigureNew(typeid(T), CreationFlags::FromScript, [&]()
			{
				ptr.emplace(std::make_shared<T>(std::forward<TArgs>(args)...));
			});
			return std::move(ptr).value();
		}

	protected:
		template<class U>
		std::shared_ptr<U> SharedFromThis(this U&& u)
		{
			return std::static_pointer_cast<U>(u.shared_from_this());
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