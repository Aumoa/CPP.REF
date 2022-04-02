// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "AssemblyInfo.Asp.h"

#include "DependencyInjection/ServiceCollection.h"

DEFINE_ASSEMBLY_INFO;

GENERATE_BODY(libty::Asp::DependencyInjection::IServiceCollection);
GENERATE_BODY(libty::Asp::DependencyInjection::SServiceCollection);
GENERATE_BODY(libty::Asp::DependencyInjection::SServiceCollectionExtensions);

GENERATE_BODY(libty::Asp::Configuration::IConfiguration);
GENERATE_BODY(libty::Asp::Configuration::IConfigurationSection);

GENERATE_BODY(libty::Asp::RestApi::IActionResult);
GENERATE_BODY(libty::Asp::RestApi::SActionResult);
GENERATE_BODY(libty::Asp::RestApi::SObjectResult);
GENERATE_BODY(libty::Asp::RestApi::SOkObjectResult);
GENERATE_BODY(Attributes::libty::Asp::SMethodAttributeHttpGet);
GENERATE_BODY(Attributes::libty::Asp::SMethodAttributeProducesResponseType);

GENERATE_BODY(libty::Asp::Builder::SAspApplication);
GENERATE_BODY(libty::Asp::Builder::SAspApplicationBuilder);

GENERATE_BODY(libty::Asp::Controllers::SControllerBase);
GENERATE_BODY(Attributes::libty::Asp::SClassAttributeRoute);