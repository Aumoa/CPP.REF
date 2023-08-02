// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module CoreAObject:Object;

import Core;

ACLASS()
class COREAOBJECT_API AObject
{
	GENERATED_BODY()

private:
	struct Referencer
	{
		int32 Refs;
		int32 Weaks;
	};

	struct GCCollectorMarks
	{
		bool bMarkReferenced : 1 = false;
	};

private:
	Referencer* Refs = nullptr;
	GCCollectorMarks GCM;

public:
	AObject();
	virtual ~AObject() noexcept;

	AFUNCTION()
	virtual String ToString();
};