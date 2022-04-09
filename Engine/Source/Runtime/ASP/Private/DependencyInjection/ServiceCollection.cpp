// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DependencyInjection/ServiceCollection.h"

using namespace ::libty::Asp::DependencyInjection;

SServiceCollection::SServiceCollection()
	: Super()
{
}

IServiceCollection* SServiceCollection::Configure(SType* optionsGenericType, IConfiguration* configuration)
{
	throw NotImplementedException();
}

IServiceCollection* SServiceCollection::AddController(SType* controllerGenericType)
{
	_controllers.emplace(controllerGenericType);
	return this;
}