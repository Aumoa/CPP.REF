// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#define ACLASS(...)

#define AYLA__COMBINE_3_MACROS(V1, V2, V3) V1 ## __ ## V2 ## __ ## V3

#define GENERATED_BODY__DEFAULT_BODY(ClassName) \
private: \
	using Super = This; \
	using This = ClassName;

#define GENERATED_BODY__GATHER_PROPERTIES_BEGIN() \
protected: \
	virtual void GatherProperties(PropertyCollector& collection) override \
	{ \
		Super::GatherProperties(collection);

#define GENERATED_BODY__GATHER_PROPERTIES_PROP(Name) \
		collection.Add(TEXT(#Name), Name);

#define GENERATED_BODY__GATHER_PROPERTIES_END() \
	}

#define GENERATED_BODY__IMPL(V1, V2, V3) AYLA__COMBINE_3_MACROS(V1, V2, V3)

#define GENERATED_BODY(...) GENERATED_BODY__IMPL(GENERATED_BODY__IMPL, GENERATED_BODY__FILE_ID__, __LINE__)

#define APROPERTY(...)
