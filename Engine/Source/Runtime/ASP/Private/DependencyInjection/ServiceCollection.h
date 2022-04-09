// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DependencyInjection/IServiceCollection.h"

namespace libty::inline Asp
{
	class SAspApplication;

	class SServiceCollection : extends(SObject), implements(IServiceCollection)
	{
		GENERATED_BODY(SServiceCollection);
		friend class ::libty::Asp::SAspApplication;

	private:
		SPROPERTY(_controllers)
		std::set<SType*> _controllers;

	public:
		SServiceCollection();

		virtual IServiceCollection* Configure(SType* optionsGenericType, IConfiguration* configuration) override;
		virtual IServiceCollection* AddController(SType* controllerGenericType) override;
	};
}