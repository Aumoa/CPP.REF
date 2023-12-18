// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CodeGen/TypeGen.h"

namespace ObjectMacros
{
	template<class T>
	concept IsInstantiatable = requires
	{
		{ new T() };
	};

	template<class T>
	void STATIC_GENERATE_INTRINSIC_CLASS_METADATA(NTypeGen::NClassMetadata& Output, String NonPrefixName, EClassMetadata InClassMeta)
	{
		Output.ClassName = NonPrefixName;
		Output.Constructor = +[]() -> AObject*
		{
			if constexpr (ObjectMacros::IsInstantiatable<T>)
			{
				return new T;
			}
			else
			{
				checkf(false, TEXT("Class {} is not instantiable."), String::FromLiteral(typeid(T).name()));
				return nullptr;
			}
		};
		Output.ClassMeta = InClassMeta;
	}
}

#define REGISTER_INTRINSIC_CLASS(Class, PackageName, ...) \
	static NTypeGen::NCompiledInDefer Z__CompiledInDefer__ ## Class(Class::StaticClass(), PackageName __VA_OPT__(, __VA_ARGS__));

#define GENERATE_INTRINSIC_CLASS_METADATA(ClassNonPrefix, InClassMeta) \
	NTypeGen::NClassMetadata Metadata; \
	ObjectMacros::STATIC_GENERATE_INTRINSIC_CLASS_METADATA<A ## ClassNonPrefix>(Metadata, TEXT(#ClassNonPrefix), InClassMeta);

#define ACLASS(...)

#define BODY_MACRO_COMBINE_INNER(A, B, C, D, E) A ## B ## C ## D ## E
#define BODY_MACRO_COMBINE(A, B, C, D, E) BODY_MACRO_COMBINE_INNER(A, B, C, D, E)

#define GENERATED_BODY(...) BODY_MACRO_COMBINE(GENERATED_, CURRENT_FILE_ID, _, __LINE__, _BODY)

#define DECLARE_INTRINSIC_CLASS_BODY_CORE(ThisClass, SuperClass)		\
	public:																\
		using Super = SuperClass;										\
																		\
	public:																\
		static AType* StaticClass();									\
																		\
	private:

#define AFUNCTION(...)
