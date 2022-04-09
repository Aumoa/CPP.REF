// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Asp
{
	class SAspApplicationBuilder;
	class SControllerBase;
	class SServiceCollection;

	class ASP_API SAspApplication : extends(SObject)
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