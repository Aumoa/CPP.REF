// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Platform/PlatformAtomics.h"

namespace Ayla
{
	struct Object::CreationHack
	{
		static thread_local CreationHack s_Hack;
		bool AllowConstruct = false;
	};

	thread_local Object::CreationHack Object::CreationHack::s_Hack;
	Object::RootCollection Object::s_RootCollection;

	int64 Object::RootCollection::AddObject(Object* object)
	{
		auto lock = std::unique_lock(m_Mutex);
		int64 instanceID;
		RootMark* mark;
		if (m_InstanceIDPoolSize > 0)
		{
			instanceID = m_InstanceIDPool[m_InstanceIDPoolSize - 1];
			--m_InstanceIDPoolSize;
			mark = &m_Roots[instanceID - 1];
		}
		else
		{
			instanceID = m_Roots.size() + 1;
			mark = &m_Roots.emplace_back();
		}

		mark->Ptr = object;
		mark->Version = 0;
		PlatformAtomics::InterlockedExchange(&mark->Refs, 1);

		return instanceID;
	}

	Object* Object::RootCollection::FinalizeObject(RootMark& mark)
	{
		check(mark.Refs == 0);
		auto* ptr = mark.Ptr;
		if (m_InstanceIDPoolSize < m_InstanceIDPool.size())
		{
			m_InstanceIDPool[m_InstanceIDPoolSize++] = mark.Ptr->m_InstanceID;
		}
		else
		{
			m_InstanceIDPool.emplace_back(mark.Ptr->m_InstanceID);
			++m_InstanceIDPoolSize;
		}
		mark.Ptr = nullptr;
		mark.Version = 0;
		return ptr;
	}

	Object::RootMark& Object::RootCollection::GetMark(Object* object)
	{
		return m_Roots[object->m_InstanceID - 1];
	}

	Object::Object()
		: m_PPtrCollection(this)
	{
		GC::TrapInitialize();

		if (CreationHack::s_Hack.AllowConstruct == false)
		{
			throw InvalidOperationException(TEXT("Object must be created with Ayla::Object::New<T> function."));
		}

		m_InstanceID = s_RootCollection.AddObject(this);
	}

	Object::~Object() noexcept
	{
		GC::EnsureFinalizerThread();
	}

	void Object::ConfigureNew(std::function<Object*()> action)
	{
		CreationHack::s_Hack.AllowConstruct = true;
		auto object = action();
		object->GatherProperties(object->m_PPtrCollection);
		CreationHack::s_Hack.AllowConstruct = false;
	}
}