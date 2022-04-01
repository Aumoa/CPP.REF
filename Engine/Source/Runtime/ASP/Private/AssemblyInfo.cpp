// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"

#include "DependencyInjection/IServiceCollection.h"

#include "Configuration/IConfiguration.h"
#include "Configuration/IConfigurationSection.h"

#include "RestApi/IActionResult.h"

DEFINE_ASSEMBLY_INFO;

GENERATE_BODY(libty::Asp::DependencyInjection::IServiceCollection);

GENERATE_BODY(libty::Asp::Configuration::IConfiguration);
GENERATE_BODY(libty::Asp::Configuration::IConfigurationSection);

GENERATE_BODY(libty::Asp::RestApi::IActionResult);