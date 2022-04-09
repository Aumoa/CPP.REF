// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::Asp::inline Configuration
{
	struct IConfiguration;
}

namespace libty::Asp::inline Controllers
{
	class SControllerBase;
}

namespace libty::Asp::inline DependencyInjection
{
	struct ASP_API IServiceCollection : virtual public SObject
	{
		GENERATED_BODY(IServiceCollection);

	public:
		virtual IServiceCollection* Configure(SType* optionsGenericType, IConfiguration* configuration) = 0;
		virtual IServiceCollection* AddController(SType* controllerGenericType) = 0;

		template<std::derived_from<SObject> TOptions>
		IServiceCollection* Configure(IConfiguration* configuration)
		{
			return Configure(typeof(TOptions), configuration);
		}

		template<std::derived_from<SControllerBase> TController>
		IServiceCollection* AddController()
		{
			return AddController(typeof(TController));
		}
	};
}