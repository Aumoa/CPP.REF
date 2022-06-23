// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GC.h"
#include "CoreAssert.h"
#include "Threading/Thread.h"
#include "GC.gen.cpp"

struct GC::InternalCollection
{
	std::vector<Object*> ObjectArray;
	std::vector<size_t> IndexQueue;
	size_t Count = 0;

	void AddObject(Object* internalObject)
	{
		if (IndexQueue.size() > 0)
		{
			size_t index = IndexQueue.back();
			IndexQueue.erase(IndexQueue.rbegin().base());
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
	}
};

int32 GC::sGCThreadId;
GC::InternalCollection GC::sCollection;

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
		sGCThreadId = 0;
	}
}

void GC::Collect()
{
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

void GC::UnregisterObject(Object* internalObject)
{
	check(internalObject->InternalIndex != -1);
	check(Thread::GetCurrentThread().GetThreadId() == sGCThreadId);
	sCollection.RemoveObject(internalObject);
}