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
			EServiceType ServiceType;
			SType* Class = nullptr;
			std::function<SObject* (IServiceProvider*)> Factory;
			SObject* Instanced = nullptr;
		};

	private:
		SPROPERTY(_primaryService)
		static SObjectFactory* _primaryService;
		SPROPERTY(_instanced)
		std::set<SObject*> _instanced;
		std::map<size_t, InjectionInfo> _injections;
		std::vector<IHostedService*> _hostedServices;

		Spinlock _lock;
		TaskCompletionSource<> _tcs;

	private:
		SObjectFactory();
		SObject* CreateInstance(const InjectionInfo& info);
		SObject* InternalGetService(SType* type, bool nolock = false);
		void SetAsPrimary();

	public:
		Task<> StartAsync(std::stop_token cancellationToken = {});
		Task<> StopAsync(std::stop_token cancellationToken = {});
		Task<> GetServiceTask();

		virtual SObject* GetService(SType* type) override;
		virtual SObject* Create(SType* type, std::function<SObject*(IServiceProvider*)> factory = nullptr) override;

		using IServiceProvider::GetService;
		using IServiceProvider::Create;

	public:
		static IServiceProvider* GetPrimaryService();
		static SObjectFactoryBuilder* CreateBuilder();
	};
}