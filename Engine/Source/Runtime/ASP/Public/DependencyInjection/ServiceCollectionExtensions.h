// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Asp
{
	struct IServiceCollection;

	class ASP_API SServiceCollectionExtensions : implements(SObject)
	{
		GENERATED_BODY(SServiceCollectionExtensions);

	public:
		static IServiceCollection* AddControllers(IServiceCollection* collection);
	};
}