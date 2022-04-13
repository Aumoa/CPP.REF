// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Builder/AspApplication.h"
#include "Builder/AspApplicationBuilder.h"
#include "DependencyInjection/ServiceCollection.h"
#include "Controllers/ControllerBase.h"
#include "RestApi/OkObjectResult.h"

using namespace ::libty;
using namespace ::libty::Reflection;

SAspApplication::SAspApplication()
	: Super()
{
}

SAspApplication::~SAspApplication()
{
	SE_LOG(LogTemp, Verbose, L"~SAspApplication");
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
	size_t prev = 0;
	GC.PreGarbageCollect += [&prev]()
	{
		prev = GC.NumObjects();
	};

	GC.PostGarbageCollect += [&prev]()
	{
		size_t now = GC.NumObjects();
		SE_LOG(LogTemp, Verbose, L"GC Stat: Objects {} -> {} ({})", prev, now, (int32)now - (int32)prev);
	};

	_socket = gcnew SSocket(EAddressFamily::InterNetwork, ESocketType::Stream, EProtocolType::TCP);
	_socket->Bind(IPEndPoint::Parse(L"0.0.0.0:5001"));

	SMethodInfo* method = _controllers[0]->GetType()->GetMethod(L"GetAsync");
	SObject* returnObj = method->Invoke(_controllers[0], {});
	SOkObjectResult* returnValue = nullptr;
	if (method->GetReturnType()->IsA<Task<>>())
	{
		Task<IActionResult*> returnTask = Cast<Task<IActionResult*>>(returnObj);
		returnValue = Cast<SOkObjectResult>(returnTask.GetResult());
	}
	else
	{
		returnValue = Cast<SOkObjectResult>(returnObj);
	}

	_socket->Listen();
	while (true)
	{
		try
		{
			SSocket* client = _socket->Accept();
			HandleClient(client);

			Task<>::Run([client]()
			{
				std::vector<char> buf(1024);
				size_t read = client->Recv(buf.data(), 1024);

				SE_LOG(LogTemp, Verbose, L"Received: {} bytes", read);
				client->Close();

				SE_LOG(LogTemp, Verbose, L"Session closed.");
			});
		}
		catch (const std::exception& e)
		{
			SE_LOG(LogTemp, Error, L"Exception caught: {0}", String::AsUnicode(e.what()));
		}

		GC.Hint();
	}

	return 0;
}

SAspApplicationBuilder* SAspApplication::CreateBuilder(const CommandLine& args)
{
	return gcnew SAspApplicationBuilder();
}

Task<> SAspApplication::HandleClient(SSocket* client)
{
	//SharedPtr lock = client;

	//while (true)
	//{
	//	std::array<char, 1024> buf = {};
	//	size_t read = co_await client->Recv(buf.data(), buf.size());
	//}
	co_return;
}