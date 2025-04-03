// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Object.h"
#include "TypeNotFoundException.h"
#include "Platform/PlatformAtomics.h"
#include "Reflection/TypeRegister.h"
#include "Reflection/TypeCollector.h"
#include "Reflection/ReflectionMacros.h"
#include "GC/GCPtr.Impl.h"

ACLASS__IMPL_CLASS_REGISTER_2(Ayla, Object);

namespace Ayla
{
	struct Object::CreationHack
	{
		static thread_local CreationHack s_Hack;
		bool AllowConstruct = false;
		Type* ObjectType;
	};

	thread_local Object::CreationHack Object::CreationHack::s_Hack;
	size_t Object::s_LiveObjects;
	Object::RootCollection Object::s_RootCollection;

	Object::RootCollection::RootCollection()
		: m_Roots(G1Size + G2Size)
		, m_InstanceIndexPool{ std::vector<int32>(G1Size, (int32)0), std::vector<int32>(G2Size, (int32)0) }
	{
		for (size_t i = 0; i < G1Size; ++i)
		{
			m_InstanceIndexPool[0][i] = (int32)i;
		}

		for (size_t i = 0; i < G2Size; ++i)
		{
			m_InstanceIndexPool[1][i] = (int32)(i + G1Size);
		}
	}

	int32 Object::RootCollection::AddObject(Object* object)
	{
		auto lock = std::unique_lock(m_Mutex);
		if (m_InstanceIndexPool[0].empty())
		{
			GC::InternalCollect(0, true);
		}

		auto rb = m_InstanceIndexPool[0].rbegin();
		int32 instanceIndex = *rb;
		m_InstanceIndexPool[0].erase((rb + 1).base());
		RootMark& mark = m_Roots[instanceIndex];

		mark.Ptr = object;
		mark.Version = 0;
		PlatformAtomics::InterlockedExchange(&mark.Refs, 1);

		return instanceIndex;
	}

	Object* Object::RootCollection::FinalizeObject(RootMark& mark)
	{
		check(mark.Refs == 0);
		auto* ptr = mark.Ptr;
		size_t generation = mark.Ptr->m_InstanceIndex < G1Size ? 0 : (mark.Ptr->m_InstanceIndex < G1Size + G2Size ? 1 : 2);
		m_InstanceIndexPool[generation].emplace_back(mark.Ptr->m_InstanceIndex);
		mark.Ptr->m_InstanceIndex = -1;
		mark.Ptr = nullptr;
		mark.Version = 0;
		return ptr;
	}

	Object::RootMark& Object::RootCollection::GetMark(Object* object)
	{
		return m_Roots[object->m_InstanceIndex];
	}

	Object::Object(CreationFlags flags)
		: m_Type(nullptr)
	{
		if (flags != CreationFlags::Static)
		{
			if (CreationHack::s_Hack.AllowConstruct == false)
			{
				throw InvalidOperationException(TEXT("Object must be created with Ayla::Object::New<T> function."));
			}

			PlatformAtomics::InterlockedIncrement(&s_LiveObjects);
			m_InstanceIndex = s_RootCollection.AddObject(this);
			m_Type = CreationHack::s_Hack.ObjectType;
		}
	}

	Object::Object() : Object(CreationFlags::None)
	{
	}

	Object::~Object() noexcept
	{
		checkf(m_InstanceIndex == -1, TEXT("Object is not managed by the GC."));
		checkf(m_FinalizeSuppressed, TEXT("Object was deleted outside the GC system."));
		PlatformAtomics::InterlockedDecrement(&s_LiveObjects);
	}

	String Object::ToString()
	{
		return String::Format(TEXT("{}"), String::FromLiteral(typeid(*this).name()));
	}

	void Object::ConfigureNew(const std::type_info& typeInfo, std::function<Object*()> action)
	{
		CreationHack::s_Hack.AllowConstruct = true;
		CreationHack::s_Hack.ObjectType = TypeCollector::FindType(typeInfo);
		if (CreationHack::s_Hack.ObjectType == nullptr)
		{
			throw new TypeNotFoundException(String::FromLiteral(typeInfo.name()));
		}
		auto object = action();
		auto lock = std::unique_lock(s_RootCollection.m_Mutex);
		lock.unlock();
		GC::Release(object);  // This corresponds to the AddRef called in the Object constructor.
		CreationHack::s_Hack.AllowConstruct = false;
	}
}