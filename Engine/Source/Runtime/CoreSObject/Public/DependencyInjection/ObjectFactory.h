// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "IServiceProvider.h"
#include "ServiceType.h"

namespace libty::inline Core
{
	class SObjectFactoryBuilder;
	struct IHostedService;

	class CORESOBJECT_API SObjectFactory : implements(SObject, IServiceProvider)
	{
		GENERATED_BODY(SObjectFactory);
		friend class SObjectFactoryBuilder;

	public:
		struct InjectionInfo
		{
			GENERATED_BODY(InjectionInfo);

		public:
			EServiceType ServiceType;
			SType* Class = nullptr;
			std::function<SObject* (IServiceProvider*)> Factory;
			SPROPERTY(Instanced)
			SObject* Instanced = nullptr;
		};

	private:
		std::map<size_t, InjectionInfo> _injections;
		std::vector<IHostedService*> _hostedServices;

		Spinlock _lock;
		TaskCompletionSource<> _tcs;

	private:
		SObjectFactory();
		SObject* CreateInstance(const InjectionInfo& info);
		SObject* InternalGetService(SType* type, bool nolock = false);

	public:
		Task<> StartAsync(std::stop_token cancellationToken = {});
		Task<> StopAsync(std::stop_token cancellationToken = {});
		Task<> GetServiceTask();

		virtual SObject* GetService(SType* type) override;

	public:
		static SObjectFactoryBuilder* CreateBuilder();
	};
}