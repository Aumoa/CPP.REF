// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::Asp::inline DependencyInjection
{
	struct IServiceCollection;
	class SServiceCollection;
}

namespace libty::Asp::inline Builder
{
	class SAspApplication;

	class ASP_API SAspApplicationBuilder : virtual public SObject
	{
		GENERATED_BODY(SAspApplicationBuilder);
		friend class SAspApplication;

	private:
		SPROPERTY(_services)
		DependencyInjection::SServiceCollection* _services = nullptr;

	private:
		SAspApplicationBuilder();

	public:
		virtual IServiceCollection* GetServices();

		SAspApplication* Build();
	};
}