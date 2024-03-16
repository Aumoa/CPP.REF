// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SourceCode/CppHeaderCode.h"
#include "Parser/CppSyntaxTree.h"

CppHeaderCode::CppHeaderCode(FileReference file) noexcept
	: Super(file)
{
}

Task<> CppHeaderCode::CompileAsync(CancellationToken cancellationToken)
{
	String content = co_await ReadContentAsync();
	CppSyntaxTree syntaxTree(GetSourceFile(), content);
	syntaxTree.Parse();
}

bool CppHeaderCode::TryGenerateCode(const FileReference& file, std::unique_ptr<SourceCode>* generatedCode)
{
	*generatedCode = std::make_unique<CppHeaderCode>(file);
	return true;
}