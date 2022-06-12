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
	BuildGraph graph1(TEXT("Core"), TEXT(R"(F:\CPP.REF\Engine\Intermediate\Include\Core)"));
	graph1.AddProject(TEXT(R"(F:\CPP.REF\Engine\Source\Runtime\Core)"));
	graph1.Compile().GetResult();

	BuildGraph graph2(TEXT("Console"), TEXT(R"(F:\CPP.REF\Engine\Intermediate\Include\Console)"));
	graph2.AddProject(TEXT(R"(F:\CPP.REF\Engine\Source\Runtime\Console)"));
	graph2.Compile().GetResult();

	BuildGraph graph3(TEXT("NativeReflector"), TEXT(R"(F:\CPP.REF\Engine\Intermediate\Include\NativeReflector)"));
	graph3.AddProject(TEXT(R"(F:\CPP.REF\Engine\Source\NativeApps\NativeReflector)"));
	graph3.Compile().GetResult();

	return 0;
}