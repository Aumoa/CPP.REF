// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DependencyInjection/ServiceCollection.h"

GENERATE_BODY(libty::Asp::DependencyInjection::SServiceCollection);

using namespace libty::Asp::DependencyInjection;

SServiceCollection::SServiceCollection()
	: Super()
{
}

IServiceCollection* SServiceCollection::Configure(SType* optionsGenericType, IConfiguration* configuration)
{
	throw not_implemented();
}

IServiceCollection* SServiceCollection::AddController(SType* controllerGenericType)
{
	_controllers.emplace(controllerGenericType);
	return this;
}