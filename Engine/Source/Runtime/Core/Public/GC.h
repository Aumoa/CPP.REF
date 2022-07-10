// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "GC.generated.h"

class Core;
class ConstructorInfo;

SCLASS()
class CORE_API GC : virtual public Object
{
	GENERATED_BODY()
	friend class Core;
	friend class ConstructorInfo;

private:
	struct InternalCollection;
	struct MarkingCollection;

private:
	static int32 sGCThreadId;
	static InternalCollection sCollection;
	static MarkingCollection sMarkingCol;

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
	static void WaitPendingRemoves();
};

#define gcnew GC::gcnew_binder() << new 