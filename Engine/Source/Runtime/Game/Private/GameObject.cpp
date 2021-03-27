// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameObject.h"

#include "Logging/LogMacros.h"

GGameObject::GGameObject() : Super()
	, world(nullptr)
{

}

GGameObject::~GGameObject()
{

}

void GGameObject::SetWorld(World* value)
{
	if (world != nullptr)
	{
		SE_LOG(LogGameObject, Error, L"Previous world is not nullptr.");
		return;
	}

	world = value;
}

World* GGameObject::GetWorld() const
{
	return world;
}

void GGameObject::SetName(TRefPtr<String> value)
{
	name = value;
}

TRefPtr<String> GGameObject::GetName() const
{
	return name;
}