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
			Object* const m_This;
			std::vector<BasePtr*> m_PPtrMembers;

		public:
			inline PPtrCollection(Object* this_) : m_This(this_)
			{
			}

			template<class T>
			inline void Add(PPtr<T>& pptr)
			{
				m_PPtrMembers.emplace_back(&static_cast<BasePtr&>(pptr));
			}
		};

	private:
		struct RootMark
		{
			Object* Ptr;
			int64 Refs;
			int64 Version;
		};

		class RootCollection
		{
			friend ::Ayla::GC;

			std::mutex m_Mutex;
			std::vector<RootMark> m_Roots;
			std::vector<int64> m_InstanceIDPool;
			size_t m_InstanceIDPoolSize = 0;

		public:
			int64 AddObject(Object* object);
			Object* FinalizeObject(RootMark& mark);
			RootMark& GetMark(Object* object);
		};

	private:
		static RootCollection s_RootCollection;

		PPtrCollection m_PPtrCollection;
		int64 m_InstanceID = -1;
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
		virtual ~Object() noexcept;

	public:
		template<std::derived_from<Object> T, class... TArgs>
		static RPtr<T> New(TArgs&&... args) requires std::constructible_from<T, TArgs...>
		{
			RPtr<T> ptr;
			ConfigureNew([&]()
			{
				ptr = new T(std::forward<TArgs>(args)...);
				return ptr.Get();
			});
			return ptr;
		}

	private:
		static void ConfigureNew(std::function<Object*()> action);
	};
}