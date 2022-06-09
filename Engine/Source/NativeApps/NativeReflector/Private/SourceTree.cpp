// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SourceTree.h"
#include "IO/FileReference.h"
#include "Misc/StringBuilder.h"
#include "Misc/Char.h"
#include <array>

#include "Logging/Log.h"
#include "LogCore.h"

SourceTree::SourceTree(const DirectoryReference& projectDir)
	: _projectDir(projectDir)
{
}

void SourceTree::AddDependencySourceTree(std::shared_ptr<SourceTree> sourceTree)
{
	_dependencies.emplace_back(std::move(sourceTree));
}

Task<> SourceTree::CompileAsync(std::stop_token sToken)
{
	static auto _IsSourceCode = [](const FileReference& fr) -> bool
	{
		return fr.CompareExtensions(TEXT(".cpp"), TEXT(".h"));
	};

	std::vector<FileReference> sourceCodes = _projectDir.GetAllFiles(true);

	// test
	//sourceCodes.clear();
	//sourceCodes.emplace_back(TEXT("F:\\CPP.REF\\Engine\\Source\\Runtime\\Core\\Public\\Object.h"));

	std::vector<Task<>> tasks;
	for (auto& source : Linq::Where(&sourceCodes, _IsSourceCode))
	{
		tasks.emplace_back(CompileWorker(source, sToken));
	}

	co_await Task<>::WhenAll(tasks);
}

Task<> SourceTree::CompileWorker(FileReference fr, std::stop_token sToken)
{
	co_await Task<>::Yield();

	String textCode = fr.ReadAllText();

	size_t line = 1;
	for (size_t i = 0; i < textCode.length(); ++i)
	{
		if (textCode[i] == '/' && textCode.IsValidIndex(i + 1) && textCode[i + 1] == '/')
		{
			size_t indexOf = textCode.IndexOf(TEXT("\n"), i);
			if (indexOf == -1)
			{
				break;
			}
			i = indexOf;
			++line;
			continue;
		}

		if (textCode[i] == '\n')
		{
			++line;
			continue;
		}

		if (textCode[i] == 'S')
		{
			static constexpr size_t length = TEXT("SCLASS()").length();
			if (textCode.SubstringView(i, length) == TEXT("SCLASS()"))
			{
				Interpreter_SCLASS(textCode, i, line);
			}
		}
	}
}

void SourceTree::Interpreter_SCLASS(const String& sourceCode, size_t& indexOf, size_t& line)
{
	size_t indentLevel = 0;
	String classDeclaration;

	// Grab class declaration.
	size_t declarationIndexOf = indexOf + TEXT("SCLASS()").length();
	for (size_t i = declarationIndexOf; i < sourceCode.length(); ++i)
	{
		if (sourceCode[i] == '{')
		{
			indentLevel += 1;
		}
		else if (sourceCode[i] == '}')
		{
			indentLevel -= 1;
			if (indentLevel == -1)
			{
				throw Exception(TEXT("Unexpected '}' character."));
			}
			else if (indentLevel == 0)
			{
				classDeclaration = sourceCode.Substring(declarationIndexOf, i);
				indexOf = i + 1;
				break;
			}
		}
	}

	std::vector<String> words;
	static constexpr std::array Separators = { L'{', L'}', L'\t', L'\n', L' ', L':' };

	// Split keywords.
	for (size_t i = 0; i < classDeclaration.length(); ++i)
	{
		size_t indexOfAny = classDeclaration.IndexOfAny(Separators, i, EStringComparison::CurrentCultureIgnoreCase);
		if (indexOfAny == -1)
		{
			words.emplace_back(classDeclaration.Substring(i));
		}
		else
		{
			wchar_t wk = classDeclaration[indexOfAny];
			words.emplace_back(classDeclaration.Substring(i, indexOfAny - i));
			words.emplace_back(String(wk));
			i = indexOfAny;
		}
	}

	// Parsing.
	static constexpr std::array WhiteSpaces = { L'\t', L' ', L'\r', L'\b' };
	size_t wPos = 0;

#define TOPLEVEL_LOOP_BODY() \
String& word = words[wPos]; \
if (word == TEXT("\n")) \
{ \
	line += 1; \
	continue; \
} \
 \
if (word.Trim(WhiteSpaces).IsEmpty()) \
{ \
	continue; \
}

	// Find class key.
	String classKey = [&]() -> String
	{
		for (; wPos < words.size(); ++wPos)
		{
			TOPLEVEL_LOOP_BODY();

			if (word == TEXT("class") || word == TEXT("struct"))
			{
				++wPos;
				return word;
			}

			throw Exception(String::Format(TEXT("Syntax error: Class key does not exists. Word: {}, Line: {}"), word, line));
		}

		throw Exception(String::Format(TEXT("Syntax error: Class key does not exists. Line: {}"), line));
	}();

	// Find class name.
	bool hasParents = false;
	String className = [&]() -> String
	{
		String apiKey;
		String className;

		for (; wPos < words.size(); ++wPos)
		{
			TOPLEVEL_LOOP_BODY();

			if (word[0] == ':' || word[0] == '{')
			{
				if (word[0] == ':')
				{
					hasParents = true;
				}

				if (!className.IsEmpty())
				{
					++wPos;
					return className;
				}
				else if (apiKey.IsEmpty())
				{
					++wPos;
					return apiKey;
				}

				throw Exception(String::Format(TEXT("Syntax error: Missing class name. Word: {}, Line: {}"), word, line));
			}

			if (apiKey.IsEmpty())
			{
				apiKey = word;
				continue;
			}
			else if (className.IsEmpty())
			{
				className = word;
				continue;
			}
			
			throw Exception(String::Format(TEXT("Syntax error: Missing ':' or '{' after class name. Word: {}, Line: {}"), word, line));
		}

		throw Exception(String::Format(TEXT("Syntax error: Missing ':' or '{' after class name. Line: {}"), line));
	}();

	Log::Info(LogCore, TEXT("FindClass: ClassKey = {}, ClassName = {}"), classKey, className);
}