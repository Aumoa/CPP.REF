// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Projects/HeaderFile.h"
#include "Reflection/AClass.h"
#include "Reflection/AAssembly.h"

HeaderFile::HeaderFile(String SourceFile, String SourceCode, String IntermediatePath)
	: SourceCode(SourceFile, SourceCode)
	, IntermediatePath(IntermediatePath)
{
}

Task<> HeaderFile::SaveAsync(std::stop_token SToken)
{
	String Filename = Path::GetFileNameWithoutExtension(GetSourcePath());

	std::vector<Task<>> Tasks;
	Tasks.emplace_back(File::WriteAllTextAsync(Path::Combine(IntermediatePath, Path::ChangeExtension(Filename, TEXT(".generated.h"))), GeneratedHeader(), 0, SToken));
	Tasks.emplace_back(File::WriteAllTextAsync(Path::Combine(IntermediatePath, Path::ChangeExtension(Filename, TEXT(".gen.cpp"))), GeneratedSource(), 0, SToken));

	return Task<>::WhenAll(Tasks);
}

bool HeaderFile::Parse(const AAssembly& InAssembly)
{
	String SourceText = GetSource();
	SourceCodeLocator Locator(GetSourcePath(), SourceText);
	AssemblyRef = &InAssembly;

	while (!Locator.IsEOF())
	{
		std::unique_ptr<AType> Parsed = AType::Parse(InAssembly, Locator);
		if (Parsed == nullptr)
		{
			break;
		}

		Types.emplace_back(std::move(Parsed));
	}

	return Types.size() > 0;
}

String HeaderFile::GeneratedHeader() const
{
	String HeaderSource = TEXT(R"AA(
// Copyright 2020-2022 Aumoa.lib. All right reserved.
// This file is auto-generated by AylaHeaderTool program.
// Do NOT modify this file manually.
)AA");

	for (auto& Type : Types)
	{
		HeaderSource += GeneratedTypeHeader(Type.get());
	}

	return HeaderSource;
}

String HeaderFile::GeneratedSource() const
{
	String SourceCode = TEXT(R"AA(
// Copyright 2020-2022 Aumoa.lib. All right reserved.
// This file is auto-generated by AylaHeaderTool program.
// Do NOT modify this file manually.

#include "{0}"
)AA");

	String IncludePath = GetSourcePath().Replace(TEXT("\\"), TEXT("/"));
	SourceCode = String::Format(SourceCode, IncludePath);

	for (auto& Type : Types)
	{
		SourceCode += GeneratedTypeSource(Type.get());
	}

	return SourceCode;
}

String HeaderFile::GeneratedTypeHeader(AType* InType) const
{
	if (auto* Class = dynamic_cast<AClass*>(InType))
	{
		String FileId = Path::GetFileName(GetSourcePath()).Replace(TEXT("."), TEXT("_"));

		String BodySource = TEXT(R"AA(
#define FILE_ID {0}
#define __GENERATED_BODY__{0}__{1}__
)AA");
		
		return String::Format(BodySource, FileId, Class->GetGeneratedBodyLineNumber());
	}
	else
	{
		Misc::Unreachable();
	}
}

String HeaderFile::GeneratedTypeSource(AType* InType) const
{
	if (auto* Class = dynamic_cast<AClass*>(InType))
	{
		String BodySource = TEXT(R"AA(
extern "C"
{{
	{0} void* EXPORT_CALL {1}__{2}__Construct()
	{{
		return (AObject*)new {2}();
	}}

	{0} void EXPORT_CALL {1}__{2}__Destruct(void* NativeHandle)
	{{
		delete (AObject*)NativeHandle;
	}}

	{0} const char_t* EXPORT_CALL {1}__{2}__ToString(void* NativeHandle)
	{{
		static thread_local String LocalStringCache = (({2}*)(AObject*)NativeHandle)->ToString();
		return LocalStringCache.c_str();
	}}
}}
)AA");

		return String::Format(BodySource, AssemblyRef->GetAPIName(), AssemblyRef->GetName(), InType->GetTypeName());
	}
	else
	{
		Misc::Unreachable();
	}
}