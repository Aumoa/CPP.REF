// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "InvalidOperationException.h"
#include "AssertionMacros.h"
#include "LanguageSupportMacros.h"
#include "Platform/PlatformMacros.h"
#include "GC/PPtr.h"
#include "GC/RPtr.h"
#include "Reflection/PropertyCollector.h"
#include "Reflection/ReflectionMacros.h"
#include "Reflection/ObjectReferenceWrapper.h"
#include "Threading/Spinlock.h"
#include <vector>
#include <functional>
#include <typeinfo>

extern "C"
{
	PLATFORM_SHARED_EXPORT ::Ayla::ssize_t Ayla__Object__BeginWriteGCHandle__Injected(void* self);
	PLATFORM_SHARED_EXPORT void Ayla__Object__EndWriteGCHandle__Injected(void* self, ::Ayla::ssize_t handle);
}

namespace Ayla
{
	template<class T>
	struct PropertyGather;

	class GC;
	struct TypeRegister;
	class Type;
	class RuntimeType;

	ACLASS()
	class CORE_API Object
	{
		friend GC;
		friend TypeRegister;
		friend Type;
		friend RuntimeType;
		friend ::Ayla::ssize_t (::Ayla__Object__BeginWriteGCHandle__Injected)(void* self);
		friend void ::Ayla__Object__EndWriteGCHandle__Injected(void* self, ssize_t handle);

	public:
		using This = Object;

	private:
		struct CreationHack;

	private:
		struct RootMark
		{
			Object* Ptr;
			int16 Refs;
			int32 Version;
		};

		class RootCollection
		{
			friend ::Ayla::GC;
			friend Object;
			friend ::Ayla::ssize_t (::Ayla__Object__BeginWriteGCHandle__Injected)(void* self);
			friend void ::Ayla__Object__EndWriteGCHandle__Injected(void* self, ssize_t handle);

			static constexpr size_t G1Size = 8192;
			static constexpr size_t G2Size = 65536;
			static_assert(G2Size > G1Size);

			Spinlock m_Mutex;
			std::vector<RootMark> m_Roots;
			std::vector<int32> m_InstanceIndexPool[3];

		public:
			RootCollection();

			int32 AddObject(Object* object);
			Object* FinalizeObject(RootMark& mark);
			RootMark& GetMark(Object* object);
		};

	public:
		enum class CreationFlags
		{
			None,
			FromScript = 1 << 0
		};

		GENERATE_BITMASK_ENUM_OPERATORS_FRIEND(::Ayla::Object::CreationFlags);

	private:
		static size_t s_LiveObjects;
		static RootCollection s_RootCollection;

		int32 m_InstanceIndex = -1;
		uint8 m_FinalizeSuppressed : 1 = false;
		Type* m_Type;
		CreationFlags m_Flags;
		ssize_t m_GCHandle = 0;

	protected:
		static void GatherProperties(PropertyCollector& collection)
		{
			PLATFORM_UNREFERENCED_PARAMETER(collection);
		}

		virtual void Finalize()
		{
		}

	public:
		Object();
		Object(const Object&) = delete;
		Object(Object&&) = delete;
		virtual ~Object() noexcept;

		String ToString();
		Type* GetType() const { return m_Type; }

		ssize_t GetInstanceId() const { return reinterpret_cast<ssize_t>(this); }
		ObjectReferenceWrapper AsWrapper() const
		{
			return ObjectReferenceWrapper
			{
				.InstanceId = GetInstanceId(),
				.Handle = m_GCHandle,
				.Flags = (int32)m_Flags
			};
		}

		Object& operator =(const Object&) = delete;
		Object& operator =(Object&&) = delete;

	public:
		template<std::derived_from<Object> T, class... TArgs>
		static RPtr<T> New(TArgs&&... args)
		{
			std::optional<RPtr<T>> ptr;
			ConfigureNew(typeid(T), CreationFlags::None, [&]()
			{
				ptr.emplace(new T(std::forward<TArgs>(args)...));
				return ptr->Get();
			});
			return std::move(ptr).value();
		}

		template<std::derived_from<Object> T, class... TArgs>
		static RPtr<T> ScriptNew(TArgs&&... args)
		{
			std::optional<RPtr<T>> ptr;
			ConfigureNew(typeid(T), CreationFlags::FromScript, [&]()
			{
				ptr.emplace(new T(std::forward<TArgs>(args)...));
				return ptr->Get();
			});
			return std::move(ptr).value();
		}

	private:
		static void ConfigureNew(const std::type_info& typeInfo, CreationFlags flags, std::function<Object*()> action);

		AFUNCTION()
		static void RegisterWeakReferenceHandle(ssize_t instancePtr, ssize_t gcHandle);
	};
}