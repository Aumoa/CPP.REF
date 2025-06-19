// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#define AYLA__COMBINE_1_MACROS(V1) V1
#define AYLA__COMBINE_3_MACROS(V1, V2, V3) V1 ## __ ## V2 ## __ ## V3

#define ACLASS__NEW_CLASS_INJECTED(NamespaceName, ClassName) \
extern "C" \
{ \
	PLATFORM_SHARED_EXPORT::Ayla::ssize_t Injected__ ## NamespaceName ## __ ## ClassName ## __New() \
	{ \
		return (::Ayla::size_t)new ::Ayla::RPtr<::Ayla::Object>(::Ayla::Object::New<::NamespaceName::ClassName>()); \
	} \
}

#define ACLASS__NAMEOF_NEW_CLASS_INJECTED(NamespaceName, ClassName) \
"Injected__" #NamespaceName "__" #ClassName "__New"

#define ACLASS__IMPL_CLASS_REGISTER(NamespaceName, ClassName) \
namespace Ayla \
{ \
	int32 s_ ## NamespaceName ## _ ## ClassName ## _TypeRegister = (+[]() \
	{ \
		static TypeRegister s_TR = \
		{ \
			.Namespace = TEXT(#NamespaceName), \
			.Name = TEXT(#ClassName), \
			.FullName = TEXT(#NamespaceName "." #ClassName), \
			.TypeInfo = &typeid(NamespaceName::ClassName) \
		}; \
 \
		TypeRegister::GetPropertyCollector<NamespaceName::ClassName>(s_TR.PCollector); \
		s_TR.Register(); \
		return 0; \
	})(); \
}

#define ACLASS(...)

#define GENERATED_BODY__DEFAULT_BODY(ClassName) \
public: \
	using Super = This; \
	using This = ClassName; \
	friend ::Ayla::TypeRegister; \
	friend ::Ayla::Object;

#define GENERATED_BODY__DECLARE_GATHER_PROPERTIES() \
protected: \
	static void GatherProperties(PropertyCollector& collection);

#define GENERATED_BODY__GATHER_PROPERTIES_PROP(Name) \
		collector.Add(TEXT(#Name), ::Ayla::PropertyCollector::Advance<This>(&reinterpret_cast<This*>(0)->Name));

#define GENERATED_BODY__IMPL(V1, V2, V3) AYLA__COMBINE_3_MACROS(V1, V2, V3)

#define GENERATED_BODY(...) GENERATED_BODY__IMPL(GENERATED_BODY__IMPL, GENERATED_BODY__FILE_ID__, __LINE__)

#define APROPERTY(...)

#define AFUNCTION(...)

#define rthis (::Ayla::RPtr{ this })
