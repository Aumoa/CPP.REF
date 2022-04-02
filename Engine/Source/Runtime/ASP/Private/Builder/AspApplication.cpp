// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Builder/AspApplication.h"
#include "Builder/AspApplicationBuilder.h"
#include "DependencyInjection/ServiceCollection.h"
#include "Controllers/ControllerBase.h"

using namespace libty::Asp;

SAspApplication::SAspApplication()
	: Super()
{
}

void SAspApplication::ApplyControllers(SServiceCollection* collection)
{
	for (auto& type : collection->_controllers)
	{
		_controllers.emplace_back(Cast<SControllerBase>(type->Instantiate()));
	}
}

int32 SAspApplication::Run()
{
	

	while (true)
	{
		std::this_thread::sleep_for(10ms);
	}

	return 0;
}

SAspApplicationBuilder* SAspApplication::CreateBuilder(const CommandLine& args)
{
	return gcnew SAspApplicationBuilder();
}