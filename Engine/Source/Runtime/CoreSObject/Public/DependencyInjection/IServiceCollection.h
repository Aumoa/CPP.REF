// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "ServiceType.h"
#include <functional>

namespace libty::inline Core
{
	struct IServiceProvider;
	struct IHostedService;

	struct CORESOBJECT_API IServiceCollection
	{
		GENERATED_INTERFACE_BODY(IServiceCollection);

	public:
		virtual IServiceCollection* AddService(EServiceType type, SType* cls, std::function<SObject*(IServiceProvider*)> factory) = 0;
		virtual IServiceProvider* Build() = 0;
		IServiceCollection* AddAutoServices();

		inline IServiceCollection* AddHostedService(SType* cls)
		{
			return AddService(EServiceType::HostedService, cls, nullptr);
		}

		template<class THostedService>
		inline IServiceCollection* AddHostedService() requires
			std::derived_from<THostedService, IHostedService> &&
			std::constructible_from<THostedService>
		{
			return AddService(EServiceType::HostedService, typeof(THostedService), nullptr);
		}

		template<class TFactory>
		inline IServiceCollection* AddHostedService(TFactory factory) requires
			std::invocable<TFactory, IServiceProvider*> &&
			std::convertible_to<std::invoke_result_t<TFactory, IServiceProvider*>, SObject*> &&
			std::derived_from<std::remove_pointer_t<std::invoke_result_t<TFactory, IServiceProvider*>>, IHostedService>
		{
			using THostedService = std::remove_pointer_t<std::invoke_result_t<TFactory, IServiceProvider*>>;
			return AddService(EServiceType::HostedService, typeof(THostedService), [factory](IServiceProvider* sp) -> SObject*
			{
				return factory(sp);
			});
		}

		inline IServiceCollection* AddSingleton(SType* cls)
		{
			return AddService(EServiceType::Singleton, cls, nullptr);
		}

		template<class TService>
		inline IServiceCollection* AddSingleton() requires
			std::constructible_from<TService>
		{
			return AddService(EServiceType::Singleton, typeof(TService), nullptr);
		}

		template<class TFactory>
		inline IServiceCollection* AddSingleton(TFactory factory) requires
			std::invocable<TFactory, IServiceProvider*>&&
			std::convertible_to<std::invoke_result_t<TFactory, IServiceProvider*>, SObject*>
		{
			using TService = std::remove_pointer_t<std::invoke_result_t<TFactory, IServiceProvider*>>;
			return AddService(EServiceType::Singleton, typeof(TService), [factory](IServiceProvider* sp) -> SObject*
			{
				return factory(sp);
			});
		}

		inline IServiceCollection* AddScoped(SType* cls)
		{
			return AddService(EServiceType::Scoped, cls, nullptr);
		}

		template<class TService>
		inline IServiceCollection* AddScoped() requires
			std::constructible_from<TService>
		{
			return AddService(EServiceType::Scoped, typeof(TService), nullptr);
		}

		template<class TFactory>
		inline IServiceCollection* AddScoped(TFactory factory) requires
			std::invocable<TFactory, IServiceProvider*> &&
			std::convertible_to<std::invoke_result_t<TFactory, IServiceProvider*>, SObject*>
		{
			using TService = std::remove_pointer_t<std::invoke_result_t<TFactory, IServiceProvider*>>;
			return AddService(EServiceType::Scoped, typeof(TService), [factory](IServiceProvider* sp) -> SObject*
			{
				return factory(sp);
			});
		}
	};
}