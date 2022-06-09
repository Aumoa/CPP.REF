// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleApplication.h"
#include "NativeReflector.h"

int main(int argc, char** argv)
{
	return ConsoleApplication::GuardedMain<NativeReflector>(argc, argv);
}