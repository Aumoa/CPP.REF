// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Logging/LogCategoryBase.h"

#include "Logging/LogVerbosity.h"

LogCategoryBase::LogCategoryBase(ELogVerbosity inBaseVerbosity, TRefPtr<String> inCategoryName)
{
	CategoryName = inCategoryName;
	BaseVerbosity = inBaseVerbosity;
}

LogCategoryBase::~LogCategoryBase()
{

}

ELogVerbosity LogCategoryBase::GetAmendedVerbosity(ELogVerbosity verbosity) const
{
	if ((int32)verbosity > (int32)BaseVerbosity)
	{
		return BaseVerbosity;
	}
	else
	{
		return verbosity;
	}
}