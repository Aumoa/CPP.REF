// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AAssembly
{
private:
	String Name;
	String APIName;

public:
	AAssembly(String InAssemblyName);

	String GetName() const;
	String GetAPIName() const;
};