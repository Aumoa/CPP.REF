// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ObjectCollection.h"

std::vector<std::shared_ptr<SObject>> ObjectCollection::MarkRemoved;

void ObjectCollection::AddMarkedObject(SObject* InObject)
{
	MarkRemoved.emplace_back(InObject->shared_from_this());
}

void ObjectCollection::RemoveMarkedObject(SObject* InObject)
{
	for (auto It = MarkRemoved.begin(); It != MarkRemoved.end(); ++It)
	{
		if (It->get() == InObject)
		{
			MarkRemoved.erase(It);
			break;
		}
	}
}

void ObjectCollection::Collect()
{
	MarkRemoved.clear();
}