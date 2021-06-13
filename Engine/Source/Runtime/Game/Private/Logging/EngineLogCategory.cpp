﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineLogCategory.h"

#include "Logging/LogVerbosity.h"

LogCategoryBase LogCamera(ELogVerbosity::Verbose, nameof(LogCamera));
LogCategoryBase LogEngine(ELogVerbosity::Verbose, nameof(LogEngine));
LogCategoryBase LogActor(ELogVerbosity::Verbose, nameof(LogActor));
LogCategoryBase LogActorComponent(ELogVerbosity::Verbose, nameof(LogActorComponent));
LogCategoryBase LogDiagnostics(ELogVerbosity::Verbose, nameof(LogDiagnostics));
LogCategoryBase LogRendering(ELogVerbosity::Verbose, nameof(LogRendering));
LogCategoryBase LogPlatform(ELogVerbosity::Verbose, nameof(LogPlatform));
LogCategoryBase LogInput(ELogVerbosity::Verbose, nameof(LogInput));
LogCategoryBase LogPawn(ELogVerbosity::Verbose, nameof(LogPawn));
LogCategoryBase LogSceneComponent(ELogVerbosity::Verbose, nameof(LogSceneComponent));
LogCategoryBase LogMovement(ELogVerbosity::Verbose, nameof(LogMovement));
LogCategoryBase LogShader(ELogVerbosity::Verbose, nameof(LogShader));
LogCategoryBase LogRHI(ELogVerbosity::Verbose, nameof(LogRHI));
LogCategoryBase LogCOM(ELogVerbosity::Verbose, nameof(LogCOM));
LogCategoryBase LogWindows(ELogVerbosity::Verbose, nameof(LogWindows));
LogCategoryBase LogDirectX(ELogVerbosity::Verbose, nameof(LogDirectX));
LogCategoryBase LogGameObject(ELogVerbosity::Verbose, nameof(LogGameObject));
LogCategoryBase LogAssets(ELogVerbosity::Verbose, nameof(LogAssets));