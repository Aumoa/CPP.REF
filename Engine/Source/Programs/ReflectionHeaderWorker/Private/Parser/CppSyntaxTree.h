// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CppExpression;

class CppSyntaxTree
{
private:
	FileReference sourceFile;
	String sourceCode;
	size_t lineNumber;
	size_t charNumber;
	size_t index;
	std::vector<std::unique_ptr<CppExpression>> expressions;

public:
	CppSyntaxTree(FileReference sourceFile, String sourceCode);
	~CppSyntaxTree() noexcept;

	void Parse();
	bool HasError() const;

private:
	std::unique_ptr<CppExpression> MakeErrorExpression();
	std::unique_ptr<CppExpression> ReadNumberExpression();
	std::unique_ptr<CppExpression> ReadSingleLineComment();

private:
	template<class... Chars>
	bool CompareCharsSimple(Chars&&... values) const
	{
		return CompareCharsSimple(std::make_index_sequence<sizeof...(Chars)>{}, std::forward<Chars>(values)...);
	}

	template<size_t... Indexes, class... Chars>
	bool CompareCharsSimple(std::index_sequence<Indexes...>&&, Chars&&... values) const
	{
		return sourceCode.IsValidIndex(index + sizeof...(Chars) - 1)
			&& (true && ... && (Char::ToLower(sourceCode[index + Indexes]) == values));
	}

	void IncrementIndex(bool isNewLine);
};