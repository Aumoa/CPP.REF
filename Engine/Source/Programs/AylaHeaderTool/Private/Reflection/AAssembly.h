// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AAssembly
{
private:
	String APIName;

public:
	AAssembly(String InAPIName);

	String GetAPIName() const;
};