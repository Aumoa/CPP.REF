// Copyright 2020 Aumoa.lib. All right reserved.

#include "TickFunction.h"

#include "Logging/LogCategoryBase.h"
#include "Logging/LogVerbosity.h"

using namespace std::chrono;

LogCategoryBase TickFunction::LogTicking(ELogVerbosity::Verbose, L"LogTicking");

TickFunction::TickFunction()
	: bCanEverTick(false)
	, TickGroup(TickingGroup::PrePhysics)
	, TickInterval(0s)
{

}

TickFunction::~TickFunction()
{

}