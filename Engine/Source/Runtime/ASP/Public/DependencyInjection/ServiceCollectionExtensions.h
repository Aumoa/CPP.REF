// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::Asp::inline DependencyInjection
{
	struct IServiceCollection;

	class ASP_API SServiceCollectionExtensions : virtual public SObject
	{
		GENERATED_BODY(SServiceCollectionExtensions);

	public:
		static IServiceCollection* AddControllers(IServiceCollection* collection);
	};
}