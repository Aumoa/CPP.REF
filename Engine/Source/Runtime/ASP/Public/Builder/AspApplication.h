// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::Asp::inline DependencyInjection
{
	class SServiceCollection;
}

namespace libty::Asp::inline Controllers
{
	class SControllerBase;
}

namespace libty::Asp::inline Builder
{
	class SAspApplicationBuilder;

	class ASP_API SAspApplication : virtual public SObject
	{
		GENERATED_BODY(SAspApplication);
		friend class SAspApplicationBuilder;

	private:
		SPROPERTY(_controllers)
		std::vector<SControllerBase*> _controllers;

		SPROPERTY(_socket)
		Sockets::SSocket* _socket;

	private:
		SAspApplication();
		virtual ~SAspApplication() noexcept override;

		void ApplyControllers(SServiceCollection* collection);

	public:
		virtual int32 Run();

	public:
		static SAspApplicationBuilder* CreateBuilder(const CommandLine& args);

	private:
		Task<> HandleClient(Sockets::SSocket* client);
	};
}