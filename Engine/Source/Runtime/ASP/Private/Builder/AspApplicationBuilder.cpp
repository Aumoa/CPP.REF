// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Builder/AspApplicationBuilder.h"
#include "Builder/AspApplication.h"
#include "DependencyInjection/ServiceCollection.h"

using namespace ::libty;

SAspApplicationBuilder::SAspApplicationBuilder()
	: Super()
{
	_services = gcnew SServiceCollection();
}

IServiceCollection* SAspApplicationBuilder::GetServices()
{
	return _services;
}

SAspApplication* SAspApplicationBuilder::Build()
{
	SAspApplication* application = gcnew SAspApplication();
	application->ApplyControllers(_services);
	return application;
}