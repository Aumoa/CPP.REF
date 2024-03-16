// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SourceCode/SourceCode.h"

class CppSyntaxTree;

class CppHeaderCode : public SourceCode
{
public:
	using Super = SourceCode;

private:
	std::unique_ptr<CppSyntaxTree> syntaxTree;

public:
	CppHeaderCode(FileReference file) noexcept;
	virtual ~CppHeaderCode() noexcept override;

	virtual Task<> CompileAsync(CancellationToken cancellationToken = {}) override;
	virtual bool TryFormatError(String* outErrorStr) const override;

public:
	static bool TryGenerateCode(const FileReference& file, std::unique_ptr<SourceCode>* generatedCode);
};