// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "GC.generated.h"

class Core;

SCLASS()
class CORE_API GC : virtual public Object
{
	GENERATED_BODY()
	friend class Core;

private:
	struct InternalCollection;

private:
	static int32 sGCThreadId;
	static InternalCollection sCollection;

private:
	GC() = delete;
	static void Initialize();
	static void Shutdown();

public:
	struct gcnew_binder
	{
		template<class T>
		T* operator <<(T* ptr)
		{
			RegisterObject(ptr);
			return ptr;
		}
	};

public:
	static bool IsValidLowLevel(Object* internalObject);
	static void Collect();

private:
	static void RegisterObject(Object* internalObject);
	static void UnregisterObject(Object* internalObject);
};

#define gcnew GC::gcnew_binder() << new 