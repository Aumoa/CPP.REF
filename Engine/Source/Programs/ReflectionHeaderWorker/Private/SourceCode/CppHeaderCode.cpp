// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "SourceCode/CppHeaderCode.h"
#include "Parser/CppSyntaxTree.h"
#include "Parser/CppExpression.h"

CppHeaderCode::CppHeaderCode(FileReference file) noexcept
	: Super(file)
{
}

CppHeaderCode::~CppHeaderCode() noexcept
{
}

Task<> CppHeaderCode::CompileAsync(CancellationToken cancellationToken)
{
	String content = co_await ReadContentAsync();
	syntaxTree = std::make_unique<CppSyntaxTree>(GetSourceFile(), content);
	syntaxTree->Parse();
}

bool CppHeaderCode::TryFormatError(String* outErrorStr) const
{
	const CppExpression* expression;
	if (syntaxTree->TryGetError(&expression))
	{
		*outErrorStr = expression->Format();
		return true;
	}

	return false;
}

bool CppHeaderCode::TryGenerateCode(const FileReference& file, std::unique_ptr<SourceCode>* generatedCode)
{
	*generatedCode = std::make_unique<CppHeaderCode>(file);
	return true;
}