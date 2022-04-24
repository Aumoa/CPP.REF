// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Asp
{
	class SAspApplication;
	struct IServiceCollection;
	class SServiceCollection;

	class ASP_API SAspApplicationBuilder : implements(SObject)
	{
		GENERATED_BODY(SAspApplicationBuilder);
		friend class SAspApplication;

	private:
		SPROPERTY(_services)
		SServiceCollection* _services = nullptr;

	private:
		SAspApplicationBuilder();

	public:
		virtual IServiceCollection* GetServices();

		SAspApplication* Build();
	};
}