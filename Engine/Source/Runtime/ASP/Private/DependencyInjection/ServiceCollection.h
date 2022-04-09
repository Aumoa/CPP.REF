// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DependencyInjection/IServiceCollection.h"

namespace libty::Asp::inline Builder
{
	class SAspApplication;
}

namespace libty::Asp::inline DependencyInjection
{
	class SServiceCollection : virtual public SObject, implements(IServiceCollection)
	{
		GENERATED_BODY(SServiceCollection);
		friend class ::libty::Asp::Builder::SAspApplication;

	private:
		SPROPERTY(_controllers)
		std::set<SType*> _controllers;

	public:
		SServiceCollection();

		virtual IServiceCollection* Configure(SType* optionsGenericType, IConfiguration* configuration) override;
		virtual IServiceCollection* AddController(SType* controllerGenericType) override;
	};
}