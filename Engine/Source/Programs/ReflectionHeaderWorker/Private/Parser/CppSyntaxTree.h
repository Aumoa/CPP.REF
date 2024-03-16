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
	bool TryGetError(const CppExpression** outExpression) const;

private:
	std::unique_ptr<CppExpression> MakeErrorExpression();
	std::unique_ptr<CppExpression> ReadNumberExpression();
	std::unique_ptr<CppExpression> ReadSingleLineComment();
	std::unique_ptr<CppExpression> ReadPreprocessor();
	std::unique_ptr<CppExpression> ReadIdentifier();

private:
	template<size_t k_Length>
	bool CompareCharsSimple(const char(&values)[k_Length])
	{
		return CompareCharsSimple(std::make_index_sequence<k_Length - 1>{}, values);
	}

	template<size_t... k_Indexes>
	bool CompareCharsSimple(std::index_sequence<k_Indexes...>&&, const char* values) const
	{
		return sourceCode.IsValidIndex(index + sizeof...(k_Indexes) - 1)
			&& (true && ... && (Char::ToLower(sourceCode[index + k_Indexes]) == values[k_Indexes]));
	}

	void IncrementIndex(bool isNewLine);
};