// Copyright 2020 Aumoa.lib. All right reserved.

#include <Windows.h>

#include "PxFoundation.h"
#include "PxPhysicsVersion.h"
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"

using namespace physx;

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE _, char* cmdLine, int nShow)
{
	PxDefaultAllocator defaultAllocator;
	PxDefaultErrorCallback defaultCallback;

	auto pxf = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocator, defaultCallback);

	return 0;
}