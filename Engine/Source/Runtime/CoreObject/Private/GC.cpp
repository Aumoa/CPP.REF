// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GC.h"
//#include "CoreAssert.h"
#include "Type.h"
#include "Threading/Thread.h"
#include "Reflection/PropertyInfo.h"
#include "GC.gen.cpp"

struct GC::InternalCollection
{
	std::vector<Object*> ObjectArray;
	std::vector<Object*> PendingRemoves;
	std::vector<size_t> IndexQueue;

	void AddObject(Object* internalObject)
	{
		if (IndexQueue.size() > 0)
		{
			size_t index = IndexQueue.back();
			IndexQueue.erase(IndexQueue.end() - 1);
			ObjectArray[index] = internalObject;
			internalObject->InternalIndex = (int64)index;
		}
		else
		{
			size_t index = ObjectArray.size();
			ObjectArray.emplace_back(internalObject);
			internalObject->InternalIndex = (int64)index;
		}
	}

	void RemoveObject(Object* internalObject)
	{
		size_t index = (size_t)internalObject->InternalIndex;
		ObjectArray[index] = nullptr;
		IndexQueue.emplace_back(index);
		PendingRemoves.emplace_back(internalObject);
		internalObject->Reference->bDisposed = true;
	}
};

struct GC::MarkingCollection
{
	std::vector<Object*> MarkingObjects;
	std::vector<Object*> PendingObjects;

	void Clear(size_t count)
	{
		MarkingObjects.clear();
		MarkingObjects.reserve(count);

		PendingObjects.clear();
		PendingObjects.reserve(count);
	}

	void Add(Object*& ptr)
	{
		MarkingObjects.emplace_back(ptr);
	}

	void Swap()
	{
		MarkingObjects.swap(PendingObjects);
		PendingObjects.clear();
	}
};

int32 GC::sGCThreadId;
GC::InternalCollection GC::sCollection;
GC::MarkingCollection GC::sMarkingCol;

void GC::Initialize()
{
	check(sGCThreadId == 0);
	sGCThreadId = Thread::GetCurrentThread().GetThreadId();
}

void GC::Shutdown()
{
	if (sGCThreadId != 0)
	{
		Collect();
		WaitPendingRemoves();
		sGCThreadId = 0;
	}
}

void GC::Collect()
{
	static auto isGenericSource = [](Type* type)
	{
		// Is generic?
		if (type->IsGenericType() == false)
		{
			return false;
		}

		// Is vector?
		if (typeof(std::vector)->Equals(type->GetGenericTypeDefinition()) == false)
		{
			return false;
		}

		// Generic type is GC object?
		Type* genericArgument = type->GetGenericArguments()[0];
		if (genericArgument == nullptr || genericArgument->IsClass() == false)
		{
			return false;
		}

		return true;
	};

	// Fill initial objects.
	sMarkingCol.Clear(sCollection.ObjectArray.size());
	for (size_t i = 0; i < sCollection.ObjectArray.size(); ++i)
	{
		if (Object*& ptr = sCollection.ObjectArray[i]; ptr && ptr->IsRoot())
		{
			sMarkingCol.Add(ptr);
			ptr->bMarking = true;
		}
	}

	// Marking recursive.
	while (true)
	{
		for (size_t i = 0; i < sMarkingCol.MarkingObjects.size(); ++i)
		{
			if (Object*& ptr = sMarkingCol.MarkingObjects[i]; ptr)
			{
				Type* t = ptr->GetType();
				for (auto& prop : t->GetGCProperties())
				{
					if (Type* propertyType = prop->GetPropertyType(); isGenericSource(propertyType))
					{
						Type* genericType = propertyType->GetGenericArguments()[0];
						std::vector<void*> objects = prop->GetValue<std::vector<void*>>(ptr);
						for (auto& ptr : objects)
						{
							if (Object* obj = genericType->GetObject(ptr); obj && !obj->bMarking)
							{
								obj->bMarking = true;
								sMarkingCol.PendingObjects.emplace_back(obj);
							}
						}
					}
					else if (Object* value = prop->GetValue<Object>(ptr); value && !value->bMarking)
					{
						value->bMarking = true;
						sMarkingCol.PendingObjects.emplace_back(value);
					}
				}
			}
		}

		if (sMarkingCol.PendingObjects.size() == 0)
		{
			break;
		}

		sMarkingCol.Swap();
	}

	// Collecting objects.
	WaitPendingRemoves();

	for (size_t i = 0; i < sCollection.ObjectArray.size(); ++i)
	{
		if (auto*& ptr = sCollection.ObjectArray[i]; ptr)
		{
			if (ptr->bMarking)
			{
				ptr->bMarking = false;
			}
			else
			{
				sCollection.RemoveObject(ptr);
			}
		}
	}

	// Delete objects.
	Task<>::Run([]()
	{
		for (auto& ptr : sCollection.PendingRemoves)
		{
			delete ptr;
		}
		
		sCollection.PendingRemoves.clear();
	});
}

bool GC::IsValidLowLevel(Object* internalObject)
{
	if (internalObject == nullptr || internalObject->InternalIndex == -1)
	{
		return false;
	}

	return true;
}

void GC::RegisterObject(Object* internalObject)
{
	check(internalObject->InternalIndex == -1);
	check(Thread::GetCurrentThread().GetThreadId() == sGCThreadId);
	sCollection.AddObject(internalObject);
}

void GC::WaitPendingRemoves()
{
	while (sCollection.PendingRemoves.size())
	{
		std::this_thread::yield();
	}
}