// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "GC/PPtr.h"
#include "GC/RPtr.h"
#include "System/InvalidOperationException.h"
#include "System/AssertionMacros.h"
#include <mutex>
#include <vector>
#include <functional>

namespace Ayla
{
	namespace GC
	{
		void Collect();
	}

	class CORE_API Object
	{
		friend void GC::Collect();

	private:
		struct CreationHack;
		static thread_local CreationHack s_Hack;

	protected:
		class PPtrCollection
		{
		private:
			Object* const m_This;
			std::vector<GC::BasePtr*> m_PPtrMembers;

		public:
			inline PPtrCollection(Object* this_) : m_This(this_)
			{
			}

			template<std::derived_from<Object> T>
			inline void Add(GC::PPtr<T>& pptr)
			{
				m_PPtrMembers.emplace_back(&static_cast<GC::BasePtr&>(pptr));
			}
		};

	private:
		struct RootMark
		{
			Object* Ptr;
			int64 Refs;
			bool SuppressFinalize;
		};

		class RootCollection
		{
			std::mutex m_Mutex;
			std::vector<RootMark> m_Roots;
			std::vector<int64> m_InstanceIDPool;

		public:
			int64 AddObject(Object* object);
			void FinalizeObject(Object* object);
			RootMark& GetMark(Object* object);

			void Collect();
		};

	private:
		static RootCollection s_RootCollection;

		PPtrCollection m_PPtrCollection;
		int64 m_InstanceID;

	protected:
		Object();

		virtual void GatherProperties(PPtrCollection& collection)
		{
		}

		virtual void Finalize()
		{
		}

	public:
		virtual ~Object() noexcept
		{
		}

	public:
		template<std::derived_from<Object> T, class... TArgs>
		static GC::RPtr<T> New(TArgs&&... args) requires std::constructible_from<T, TArgs...>
		{
			GC::RPtr<T> ptr;
			ConfigureNew([&ptr]()
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