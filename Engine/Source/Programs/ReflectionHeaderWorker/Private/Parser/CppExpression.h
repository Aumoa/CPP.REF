// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CppExpression
{
private:
	const String expression;
	const FileReference sourceFile;
	const size_t lineNumber;
	const size_t charNumber;

protected:
	CppExpression(String expression, FileReference sourceFile, size_t lineNumber, size_t charNumber);

public:
	virtual ~CppExpression() noexcept;

	virtual String Format() const;

	String GetExpression() const;
	FileReference GetSourceFile() const;
	size_t GetLineNumber() const;
	size_t GetCharNumber() const;
};