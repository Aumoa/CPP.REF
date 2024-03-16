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
	std::unique_ptr<CppExpression> ReadKeyword();

private:
	template<size_t k_Length>
	bool CompareCharsSimple(const char(&values)[k_Length]) const
	{
		return CompareCharsSimple(std::make_index_sequence<k_Length - 1>{}, values);
	}

	template<size_t... k_Indexes>
	bool CompareCharsSimple(std::index_sequence<k_Indexes...>&&, const char* values) const
	{
		return sourceCode.IsValidIndex(index + sizeof...(k_Indexes) - 1)
			&& (true && ... && (Char::ToLower(sourceCode[index + k_Indexes]) == values[k_Indexes]));
	}

	template<class... ArrayTypes>
	bool CompareCharsMultiple(ArrayTypes&&... values) const
	{
		return CompareCharsMultipleInternal(std::make_index_sequence<sizeof...(ArrayTypes)>{}, std::forward<ArrayTypes>(values)...);
	}

	template<class... ArrayTypes, size_t... k_Indexes>
	bool CompareCharsMultipleInternal(std::index_sequence<k_Indexes...>&&, ArrayTypes&&... values) const
	{
		return (false ||
			... ||
			CompareCharsMultipleDispatch<0>(std::forward<ArrayTypes>(values))
			);
	}

	template<size_t k_Index, size_t k_Size>
	bool CompareCharsMultipleDispatch(const char (&value)[k_Size]) const
	{
		if (Char::ToLower(sourceCode[index + k_Index]) != value[k_Index])
		{
			return false;
		}

		if constexpr (k_Index + 1 < k_Size - 1)
		{
			return CompareCharsMultipleDispatch<k_Index + 1, k_Size>(value);
		}
		else
		{
			return true;
		}
	}

	void IncrementIndex(bool isNewLine);
};