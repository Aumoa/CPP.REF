// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "GC/PPtr.h"
#include "GC/RPtr.h"
#include "GC/GC.h"
#include "InvalidOperationException.h"
#include "AssertionMacros.h"
#include <mutex>
#include <vector>
#include <functional>

namespace Ayla
{
	template<class T>
	struct PropertyGather;

	class CORE_API Object
	{
		friend ::Ayla::GC;

	public:
		using This = Object;

	private:
		struct CreationHack;

	protected:
		class PPtrCollection
		{
			friend ::Ayla::GC;

		private:
			std::vector<std::unique_ptr<PropertyGather<void>>> m_PPtrMembers;

		public:
			PPtrCollection() noexcept = default;
			PPtrCollection(const PPtrCollection&) = delete;

			template<class T>
			inline void Add(T& pptr) requires requires { std::declval<PropertyGather<T>>(); }
			{
				m_PPtrMembers.emplace_back(std::make_unique<PropertyGather<T>>(&pptr));
			}

			PPtrCollection& operator =(const PPtrCollection&) = delete;
		};

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

			static constexpr size_t G1Size = 8192;
			static constexpr size_t G2Size = 65536;
			static_assert(G2Size > G1Size);

			std::mutex m_Mutex;
			std::vector<RootMark> m_Roots;
			std::vector<int32> m_InstanceIndexPool[3];

		public:
			RootCollection();

			int32 AddObject(Object* object);
			Object* FinalizeObject(RootMark& mark);
			RootMark& GetMark(Object* object);
		};

	private:
		static size_t s_LiveObjects;
		static RootCollection s_RootCollection;

		PPtrCollection m_PPtrCollection;
		int32 m_InstanceIndex = -1;
		bool m_FinalizeSuppressed = false;

	protected:
		virtual void GatherProperties(PPtrCollection& collection)
		{
			PLATFORM_UNREFERENCED_PARAMETER(collection);
		}

		virtual void Finalize()
		{
		}

	public:
		Object();
		Object(const Object&) = delete;
		virtual ~Object() noexcept;

		String ToString();

		Object& operator =(const Object&) = delete;

	public:
		template<std::derived_from<Object> T, class... TArgs>
		static RPtr<T> New(TArgs&&... args) requires std::constructible_from<T, TArgs...>
		{
			std::optional<RPtr<T>> ptr;
			ConfigureNew([&]()
			{
				ptr.emplace(new T(std::forward<TArgs>(args)...));
				return ptr->Get();
			});
			return std::move(ptr).value();
		}

	private:
		static void ConfigureNew(std::function<Object*()> action);
	};
}