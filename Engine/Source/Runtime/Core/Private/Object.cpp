// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Platform/PlatformAtomics.h"

namespace Ayla
{
	struct Object::CreationHack
	{
		bool AllowConstruct = false;
	};

	thread_local Object::CreationHack Object::s_Hack;

	int64 Object::RootCollection::AddObject(Object* object)
	{
		auto lock = std::unique_lock(m_Mutex);
		int64 instanceID;
		RootMark* mark;
		if (m_InstanceIDPool.size() > 0)
		{
			instanceID = m_InstanceIDPool.back();
			m_InstanceIDPool.erase(m_InstanceIDPool.end() - 1);
			mark = &m_Roots[instanceID - 1];
		}
		else
		{
			instanceID = m_Roots.size() + 1;
			mark = &m_Roots.emplace_back();
		}

		mark->Ptr = object;
		PlatformAtomics::InterlockedExchange(&mark->Refs, 1);
		mark->SuppressFinalize = false;

		return instanceID;
	}

	void Object::RootCollection::FinalizeObject(Object* object)
	{
		auto& mark = m_Roots[object->m_InstanceID - 1];
		check(mark.Refs == 0);

		auto lock = std::unique_lock(m_Mutex);
		auto& mark = m_Roots[object->m_InstanceID - 1];
		mark.Ptr = nullptr;
		m_InstanceIDPool.emplace_back(object->m_InstanceID);
	}

	Object::RootMark& Object::RootCollection::GetMark(Object* object)
	{
		return m_Roots[object->m_InstanceID - 1];
	}

	Object::Object()
		: m_PPtrCollection(this)
	{
		if (s_Hack.AllowConstruct == false)
		{
			throw InvalidOperationException(TEXT("Object must be created with Ayla::Object::New<T> function."));
		}

		m_InstanceID = s_RootCollection.AddObject(this);
	}

	void Object::ConfigureNew(std::function<Object*()> action)
	{
		s_Hack.AllowConstruct = true;
		auto object = action();
		PlatformAtomics::InterlockedDecrement(&s_RootCollection.GetMark(object).Refs);
		s_Hack.AllowConstruct = false;
	}
}