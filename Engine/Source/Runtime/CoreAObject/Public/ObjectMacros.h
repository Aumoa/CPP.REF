// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CodeGen/TypeGen.h"

#define REGISTER_INTRINSIC_CLASS(Class, PackageName) \
	static NTypeGen::NCompiledInDefer Z__CompiledInDefer__ ## Class(Class::StaticClass(), PackageName);

#define GENERATE_INTRINSIC_CLASS_METADATA(ClassNonPrefix, InClassMeta) \
	NTypeGen::NClassMetadata Metadata; \
	Metadata.ClassName = TEXT(#ClassNonPrefix); \
	Metadata.Constructor = +[]() { return (AObject*)new A ## ClassNonPrefix(); }; \
	Metadata.ClassMeta = InClassMeta;

#define ACLASS(...)

#define BODY_MACRO_COMBINE_INNER(A, B, C, D, E) A ## B ## C ## D ## E
#define BODY_MACRO_COMBINE(A, B, C, D, E) BODY_MACRO_COMBINE_INNER(A, B, C, D, E)

#define GENERATED_BODY(...) BODY_MACRO_COMBINE(GENERATED_, CURRENT_FILE_ID, _, __LINE__, _BODY)

#define DECLARE_INTRINSIC_CLASS_BODY_CORE(ThisClass, SuperClass)		\
	private:															\
		using Super = SuperClass;										\
																		\
	public:																\
		static AType* StaticClass();									\
																		\
	private:

#define AFUNCTION(...)