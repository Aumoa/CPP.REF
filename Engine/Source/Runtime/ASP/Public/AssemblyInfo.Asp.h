// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "AssemblyInfo.CoreSObject.h"
#include "AssemblyInfo.Sockets.h"

#include "DependencyInjection/IServiceCollection.h"
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