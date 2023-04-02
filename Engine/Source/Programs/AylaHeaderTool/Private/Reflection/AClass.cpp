// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/AClass.h"
#include "Reflection/AAssembly.h"
#include "Compiler/CompileErrors.h"
#include "Exceptions/TerminateException.h"

AClass::AClass(const AAssembly& InAssembly, SourceCodeLocator& Locator) : AType(InAssembly, Locator)
{
	if (String ClassKey = GetClassKey(); ClassKey != TEXT("class"))
	{
		throw TerminateException(String::Format(TEXT("{}: {}"), Locator.ToString(), CompileErrors::InvalidClassKey(ClassKey, TEXT("class"))));
	}

	String Next = Locator.GetWord(true);
	if (Next == InAssembly.GetAPIName())
	{
		// Ignore API declaration.
		Next = Locator.GetWord(true);
	}
	TypeName = Next;
	check(TypeName.IsEmpty() == false);

	SyntaxTree.Parse(TypeName, Locator);
}

AClass::~AClass() noexcept
{
}

String AClass::GetTypeName() const
{
	return TypeName;
}

int32 AClass::GetGeneratedBodyLineNumber() const
{
	return SyntaxTree.GetGeneratedBodyLineNumber();
}