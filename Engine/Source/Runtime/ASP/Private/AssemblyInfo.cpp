// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DependencyInjection/IServiceCollection.h"
#include "DependencyInjection/ServiceCollection.h"
#include "DependencyInjection/ServiceCollectionExtensions.h"

#include "Configuration/IConfiguration.h"
#include "Configuration/IConfigurationSection.h"

#include "RestApi/IActionResult.h"
#include "RestApi/ActionResult.h"
#include "RestApi/ObjectResult.h"
#include "RestApi/OkObjectResult.h"
#include "RestApi/MethodAttributeHttpGet.h"
#include "RestApi/MethodAttributeProducesResponseType.h"

#include "Builder/AspApplication.h"
#include "Builder/AspApplicationBuilder.h"

#include "Controllers/ControllerBase.h"
#include "Controllers/ClassAttributeRoute.h"

DEFINE_ASSEMBLY_INFO;

GENERATE_BODY(libty::Asp::IServiceCollection);
GENERATE_BODY(libty::Asp::SServiceCollection);
GENERATE_BODY(libty::Asp::SServiceCollectionExtensions);

GENERATE_BODY(libty::Asp::IConfiguration);
GENERATE_BODY(libty::Asp::IConfigurationSection);

GENERATE_BODY(libty::Asp::IActionResult);
GENERATE_BODY(libty::Asp::SActionResult);
GENERATE_BODY(libty::Asp::SObjectResult);
GENERATE_BODY(libty::Asp::SOkObjectResult);
GENERATE_BODY(Attributes::libty::Asp::SMethodAttributeHttpGet);
GENERATE_BODY(Attributes::libty::Asp::SMethodAttributeProducesResponseType);

GENERATE_BODY(libty::Asp::SAspApplication);
GENERATE_BODY(libty::Asp::SAspApplicationBuilder);

GENERATE_BODY(libty::Asp::SControllerBase);
GENERATE_BODY(Attributes::libty::Asp::SClassAttributeRoute);