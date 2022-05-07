// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DependencyInjection/ObjectFactory.h"
#include "DependencyInjection/ObjectFactoryBuilder.h"
#include "DependencyInjection/InjectException.h"
#include "DependencyInjection/IHostedService.h"
#include "Reflection/MethodInfo.h"

using namespace ::libty;
using namespace ::libty::Reflection;

DEFINE_LOG_CATEGORY(LogDependencyInjection);

SObjectFactory* SObjectFactory::_primaryService;

SObjectFactory::SObjectFactory()
	: _tcs(TaskCompletionSource<>::Create())
{
}

SObject* SObjectFactory::CreateInstance(const InjectionInfo& info)
{
	SObject* instanced = nullptr;
	if (info.Factory)
	{
		instanced = info.Factory(this);
	}
	else if (SMethodInfo* inject = info.Class->GetMethod(L"Inject"); inject != nullptr)
	{
		std::span params = inject->GetParameters();
		std::vector<SObject*> args;
		args.reserve(params.size());

		for (auto& type : params)
		{
			if (type->IsA<IServiceProvider>())
			{
				args.emplace_back(this);
			}
			else
			{
				SObject* instanced = InternalGetService(type, true);
				args.emplace_back(instanced);
			}
		}

		try
		{
			instanced = info.Class->Instantiate();
			inject->Invoke(instanced, args);
		}
		catch (const std::exception& e)
		{
			SE_LOG(LogDependencyInjection, Error, L"Exception detected while running 'Inject' function for create dependency injection object.\nInnerException: {0}", String::AsUnicode(e.what()));
			throw InjectException(EInjectionErrorCode::InjectInnerException, std::current_exception());
		}
	}
	else
	{
		instanced = info.Class->Instantiate();
	}

	SE_LOG(LogDependencyInjection, Info, L"Instance of {0} type generated.", info.Class->GetFullQualifiedName());
	return instanced;
}

SObject* SObjectFactory::InternalGetService(SType* type, bool nolock)
{
	size_t hash = type->GetHashCode();
	auto it = _injections.find(hash);
	if (it == _injections.end())
	{
		return nullptr;
	}

	if (it->second.ServiceType == EServiceType::Scoped)
	{
		return CreateInstance(it->second);
	}
	else
	{
		std::unique_lock lock(nolock ? std::unique_lock(_lock, std::defer_lock) : std::unique_lock(_lock));
		if (it->second.Instanced == nullptr)
		{
			it->second.Instanced = CreateInstance(it->second);
		}
		return it->second.Instanced;
	}
}

void SObjectFactory::SetAsPrimary()
{
	_primaryService = this;
}

Task<> SObjectFactory::StartAsync(std::stop_token cancellationToken)
{
	std::unique_lock lock(_lock);
	std::vector<Task<>> tasks;
	for (auto& [hash, info] : _injections)
	{
		if (info.ServiceType == EServiceType::HostedService)
		{
			auto isHostedService = [](SType* type)
			{
				return type->IsA(typeof(IHostedService));
			};

			auto* instanced = CreateInstance(info);
			if (instanced == nullptr || Linq::Contains(instanced->GetType()->GetInterfaces(), isHostedService))
			{
				throw InjectException(EInjectionErrorCode::NotSupportedType);
			}
			info.Instanced = instanced;

			tasks.emplace_back(_hostedServices.emplace_back(Cast<IHostedService>(instanced))->StartAsync(cancellationToken));
		}
	}

	lock.unlock();

	co_await Task<>::WhenAll(std::move(tasks), cancellationToken);
	SE_LOG(LogDependencyInjection, Info, L"ObjectFactory service started.");
}

Task<> SObjectFactory::StopAsync(std::stop_token cancellationToken)
{
	std::unique_lock lock(_lock);
	std::vector<Task<>> tasks;
	for (auto& s : _hostedServices)
	{
		tasks.emplace_back(s->StopAsync(cancellationToken));
	}

	lock.unlock();

	co_await Task<>::WhenAll(std::move(tasks), cancellationToken);
	SE_LOG(LogDependencyInjection, Info, L"ObjectFactory service stopped.");
	_tcs.SetResult();
}

Task<> SObjectFactory::GetServiceTask()
{
	return _tcs.GetTask();
}

SObject* SObjectFactory::GetService(SType* type)
{
	return InternalGetService(type);
}

SObject* SObjectFactory::Create(SType* type, std::function<SObject*(IServiceProvider*)> factory)
{
	InjectionInfo info;
	info.Class = type;
	info.Factory = factory;
	return CreateInstance(info);
}

SObjectFactory* SObjectFactory::GetPrimaryService()
{
	return _primaryService;
}

SObjectFactoryBuilder* SObjectFactory::CreateBuilder()
{
	return gcnew SObjectFactoryBuilder(gcnew SObjectFactory());
}