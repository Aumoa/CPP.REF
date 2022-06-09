// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "NativeReflector.h"
#include "BuildGraph.h"
#include "Logging/Log.h"
#include "Logging/LogCategory.h"
#include "IO/DirectoryReference.h"
#include <thread>

constexpr LogCategory LogNativeReflector(TEXT("LogNativeReflector"));

NativeReflector::NativeReflector()
{
}

int32 NativeReflector::Run()
{
	Log::Info(LogNativeReflector, TEXT("Application start."));

	BuildGraph graph(TEXT(R"(F:\CPP.REF\Engine\Intermediate\ReflectionCode)"));
	graph.AddProject(TEXT(R"(F:\CPP.REF\Engine\Source\Runtime\Core)"));

	return 0;
}