// Copyright 2020 Aumoa.lib. All right reserved.

import "PxFoundation.h";
import "PxPhysicsVersion.h";
import "extensions/PxDefaultAllocator.h";
import "extensions/PxDefaultErrorCallback.h";
import "assimp/Importer.hpp";

using namespace physx;

int __stdcall WinMain(void* hInstance, void* _, char* cmdLine, int nShow)
{
	PxDefaultAllocator defaultAllocator;
	PxDefaultErrorCallback defaultCallback;

	auto pxf = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocator, defaultCallback);

	Assimp::Importer importer;

	return 0;
}