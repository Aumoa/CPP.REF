// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CompiledSCLASS.h"
#include "CompiledSCONSTRUCTOR.h"
#include "CompiledSFIELD.h"
#include "CompiledSMETHOD.h"
#include "BodyAccessKey.h"
#include "Exceptions/Exception.h"

CompiledSCLASS::CompiledSCLASS()
{
}

void CompiledSCLASS::CompileSourceTree(const String& classDeclaration, size_t& line)
{
	_sclass = line;

	std::vector<String> words;
	static constexpr std::array Separators =
	{
		L'{', L'}', L'\t', L'\n', L' ',
		L':', L',', L'<', L'>', L'[', L']', L'(', L')', L'&', L';', L'*'
	};

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

	String currentWord;
	std::coroutine_handle<> coro = STree_StartCoro(currentWord, line);

	for (; wPos < words.size() && !coro.done(); ++wPos)
	{
		currentWord = words[wPos];
		if (currentWord == TEXT("\n"))
		{
			++line;
			continue;
		}
		else if (currentWord.Trim(WhiteSpaces).IsEmpty())
		{
			continue;
		}
		coro();
	}
}

void CompiledSCLASS::Generate(StringBuilder& sb, const String& fileKey)
{
	sb.AppendLineFormat(TEXT("// SCLASS for {}"), _className);
	sb.AppendLineFormat();

	sb.AppendLineFormat(TEXT("class {};"), _className);
	sb.AppendLineFormat();

	sb.AppendLineFormat(TEXT("#define __LIBTY_SCLASS{}{} \\"), fileKey, _sclass);
	sb.AppendLineFormat(TEXT("namespace libty::reflect \\"));
	sb.AppendLineFormat(TEXT("{{ \\"));
	for (auto& member : _members)
	{
		member->Generate(_className, 1, sb);
	}
	sb.AppendLineFormat(TEXT("\ttemplate<class T> requires std::same_as<T, {}> \\"), _className);
	sb.AppendLineFormat(TEXT("\tstruct reflexpr_t<T> \\"));
	sb.AppendLineFormat(TEXT("\t{{ \\"));
	sb.AppendLineFormat(TEXT("\t\tstatic constexpr String friendly_name = TEXT(\"{}\"); \\"), _className);
	sb.AppendLineFormat(TEXT("\t}}; \\"));
	sb.AppendLineFormat(TEXT("}} \\"));
	sb.AppendLineFormat();

	sb.AppendLineFormat(TEXT("#define __LIBTY_GENERATED_BODY{}{} \\"), fileKey, _generatedBody);
	sb.AppendLineFormat(TEXT("private: \\"));
	if (!_parentClass.IsEmpty())
	{
		sb.AppendLineFormat(TEXT("\tusing Super = {}; \\"), _parentClass);
	}
	if (!_interfaceTypes.empty())
	{
		sb.AppendLineFormat(TEXT("\tusing Interfaces = std::tuple<{}>; \\"), String::Join(TEXT(", "), _interfaceTypes));
	}
	sb.AppendLineFormat(TEXT("\tusing This = {}; \\"), _className);
	sb.AppendLineFormat();
}

namespace std
{
	template<class... TArgs>
	struct coroutine_traits<std::coroutine_handle<>, TArgs...>
	{
		struct promise_type
		{
			std::exception_ptr _except_ptr;

			constexpr auto initial_suspend() const noexcept
			{
				return std::suspend_never();
			}

			constexpr auto final_suspend() const noexcept
			{
				return std::suspend_always();
			}

			void unhandled_exception() const
			{
				// rethrow.
				throw;
			}

			std::coroutine_handle<> get_return_object()
			{
				return std::coroutine_handle<promise_type>::from_promise(*this);
			}

			template<class T>
			constexpr decltype(auto) yield_value(const T& suspend_mode) const noexcept
			{
				return suspend_mode;
			}

			constexpr auto yield_value(bool) const noexcept
			{
				return std::suspend_always();
			}

			void return_void()
			{
			}

			std::coroutine_handle<> coro_handle()
			{
				return std::coroutine_handle<promise_type>::from_promise(*this);
			}
		};
	};
}

std::coroutine_handle<> CompiledSCLASS::STree_StartCoro(const String& word, size_t& line)
{
	co_yield true;

	auto innerCoro = STree_ClassDeclare(word, line);
	while (!innerCoro.done())
	{
		co_yield true;
		innerCoro();
	}
}

std::coroutine_handle<> CompiledSCLASS::STree_ClassDeclare(const String& word, size_t& line)
{
	if (word != TEXT("class"))
	{
		throw Exception(TEXTF("Syntax error: SCLASS must be starts with 'class' keyword. Word: {}, Line: {}", word, line));
	}

	co_yield true;

	String apiKey = word;

	co_yield true;

	String className = word;

	co_yield true;

	std::coroutine_handle<> innerCoro;

	auto flushClass = [&]()
	{
		if (className.IsEmpty())
		{
			className = apiKey;
		}

		if (className.IsEmpty())
		{
			throw Exception(TEXTF("Syntax error: Class name does not specified. Word: {}, Line: {}", word, line));
		}

		_className = className;
	};

	while (true)
	{
		if (word == TEXT(":"))
		{
			co_yield true;

			flushClass();
			innerCoro = STree_Inheritances(word, line);
			break;
		}
		else if (word == TEXT("{"))
		{
			flushClass();
			innerCoro = STree_Body(word, line);
			break;
		}
		else if (className.IsEmpty())
		{
			className = word;
		}
		else
		{
			throw Exception(TEXTF("Syntax error: Class body does not started with '{' or ':'. Word: {}, Line: {}", word, line));
		}

		co_yield true;
	}

	while (!innerCoro.done())
	{
		co_yield true;
		innerCoro();
	}
}

std::coroutine_handle<> CompiledSCLASS::STree_Inheritances(const String& word, size_t& line)
{
	std::coroutine_handle<> innerCoro;

	while (true)
	{
		bool isVirtual = false;
		bool isPublic = false;

		if (word == TEXT("virtual"))
		{
			isVirtual = true;
			co_yield true;
		}

		if (word == TEXT("public"))
		{
			isPublic = true;
			co_yield true;
		}

		if (word == TEXT("protected") || word == TEXT("private"))
		{
			throw Exception(TEXTF("Syntax error: 'protected' or 'private' inheritance is not allowed on SCLASS instance. Word: {}, Line: {}", word, line));
		}

		if (word == TEXT("{"))
		{
			innerCoro = STree_Body(word, line);
			break;
		}

		if (word == TEXT(","))
		{
			co_yield true;
			continue;
		}

		if (word == TEXT("Object"))
		{
			if (isVirtual)
			{
				isVirtual = false;
			}
			else
			{
				throw Exception(TEXTF("Syntax error: Inheritance class for 'Object' must be virtual. Word: {}, Line: {}", word, line));
			}
		}

		if (isVirtual)
		{
			_interfaceTypes.emplace_back(word);
			co_yield true;
		}
		else
		{
			if (_parentClass.IsEmpty())
			{
				_parentClass = word;
				co_yield true;
			}
			else
			{
				throw Exception(TEXTF("Syntax error: Inheritance class that not interface type must be one. Word: {}, Line: {}", word, line));
			}
		}
	}

	while (!innerCoro.done())
	{
		co_yield true;
		innerCoro();
	}
}

std::coroutine_handle<> CompiledSCLASS::STree_Body(const String& word, size_t& line)
{
	if (word != TEXT("{"))
	{
		throw Exception(TEXTF("Syntax error: Body of SCLASS does not starts with '{' keyword. Word: {}, Line: {}", word, line));
	}

	co_yield true;

	if (word == TEXT("GENERATED_BODY"))
	{
		_generatedBody = line;

		co_yield true;

		if (word != TEXT("("))
		{
			throw Exception(TEXTF("Syntax error: GENERATED_BODY without parentheses. Word: {}, Line: {}", word, line));
		}

		co_yield true;

		if (word != TEXT(")"))
		{
			throw Exception(TEXTF("Syntax error: GENERATED_BODY without parentheses. Word: {}, Line: {}", word, line));
		}

		co_yield true;
	}
	else
	{
		throw Exception(TEXTF("Syntax error: GENERATED_BODY does not found in SCLASS. Word: {}, Line: {}", word, line));
	}

	while (true)
	{
		if (word == TEXT("public"))
		{
			_eBodyAccess = EBodyAccessKey::Public;
			co_yield true;
			if (word != TEXT(":"))
			{
				throw Exception(TEXTF("Syntax error: Unexpected keyword. Required colon(':'). Word: {}, Line: {}", word, line));
			}
			co_yield true;
		}
		else if (word == TEXT("protected"))
		{
			_eBodyAccess = EBodyAccessKey::Protected;
			co_yield true;
			if (word != TEXT(":"))
			{
				throw Exception(TEXTF("Syntax error: Unexpected keyword. Required colon(':'). Word: {}, Line: {}", word, line));
			}
			co_yield true;
		}
		else if (word == TEXT("private"))
		{
			_eBodyAccess = EBodyAccessKey::Private;
			co_yield true;
			if (word != TEXT(":"))
			{
				throw Exception(TEXTF("Syntax error: Unexpected keyword. Required colon(':'). Word: {}, Line: {}", word, line));
			}
			co_yield true;
		}
		else if (word == TEXT("friend"))
		{
			while (word != TEXT(";"))
			{
				co_yield true;
			}

			co_yield true;
		}
		else if (word == TEXT("}"))
		{
			co_yield true;
			break;
		}
		else
		{
			std::coroutine_handle<> innerCoro = STree_Member(word, line);
			while (!innerCoro.done())
			{
				co_yield true;
				innerCoro();
			}
		}
	}
}

std::coroutine_handle<> CompiledSCLASS::STree_Member(const String& word, size_t& line)
{
	bool isInline = false;
	bool isStatic = false;
	bool isConstexpr = false;
	bool isVirtual = false;

	if (word == TEXT("template"))
	{
		co_yield true;

		size_t step = 0;
		while (true)
		{
			if (word == TEXT("<"))
			{
				++step;
			}
			else if (word == TEXT(">"))
			{
				if (--step == 0)
				{
					break;
				}
			}

			co_yield true;
		}

		co_yield true;
	}

	while (true)
	{
		if (word == TEXT("inline"))
		{
			isInline = true;
			co_yield true;
			continue;
		}

		if (word == TEXT("static"))
		{
			isStatic = true;
			co_yield true;
			continue;
		}

		if (word == TEXT("constexpr"))
		{
			isConstexpr = true;
			co_yield true;
			continue;
		}

		if (word == TEXT("virtual"))
		{
			isVirtual = true;
			co_yield  true;
			continue;
		}

		break;
	}

	String returnType;
	String name;
	bool isConst = false;
	bool isRef = false;
	bool isPtr = false;

	if (word == TEXT("const"))
	{
		isConst = true;
		co_yield true;
	}

	while (true)
	{
		if (word == TEXT("&"))
		{
			isRef = true;
			co_yield true;
			continue;
		}

		if (word == TEXT("*"))
		{
			isPtr = true;
			co_yield true;
			continue;
		}

		if (returnType.IsEmpty())
		{
			returnType = word;
			co_yield true;
			continue;
		}

		break;
	}

	auto assignMember = [&](CompiledSMEMBER* ptr)
	{
		ptr->MemberName = name;
		ptr->bInline = isInline;
		ptr->bStatic = isStatic;
		ptr->eBodyAccessKey = _eBodyAccess;
	};

	if (word == TEXT("("))
	{
		name = std::move(returnType);
		std::unique_ptr ptr = std::make_unique<CompiledSCONSTRUCTOR>();
		assignMember(ptr.get());
		std::coroutine_handle<> innerCoro = STree_Constructor(ptr.get(), word, line);
		while (!innerCoro.done())
		{
			co_yield true;
			innerCoro();
		}

		_members.emplace_back(std::move(ptr));
		co_return;
	}

	name = word;
	co_yield true;

	if (word == TEXT("=") || word == TEXT(";"))
	{
		std::unique_ptr ptr = std::make_unique<CompiledSFIELD>();
		assignMember(ptr.get());
		ptr->FieldType = returnType;
		std::coroutine_handle<> innerCoro = STree_Field(ptr.get(), word, line);
		while (!innerCoro.done())
		{
			co_yield true;
			innerCoro();
		}

		_members.emplace_back(std::move(ptr));
	}
	else if (word == TEXT("("))
	{
		std::unique_ptr ptr = std::make_unique<CompiledSMETHOD>();
		assignMember(ptr.get());
		ptr->ReturnType = returnType;
		ptr->bVirtual = isVirtual;
		std::coroutine_handle<> innerCoro = STree_Method(ptr.get(), word, line);
		while (!innerCoro.done())
		{
			co_yield true;
			innerCoro();
		}

		_members.emplace_back(std::move(ptr));
	}
}

std::coroutine_handle<> CompiledSCLASS::STree_Constructor(CompiledSCONSTRUCTOR* ptr, const String& word, size_t& line)
{
	if (word != TEXT("("))
	{
		throw Exception(TEXTF("Syntax error: Unexpected identifier between constructor name and '('. Word: {}, Line: {}", word, line));
	}

	co_yield true;

	std::vector<String> argument;
	auto flushArgument = [&]()
	{
		if (!argument.empty())
		{
			ptr->Args.emplace_back(String::Join(TEXT(" "), argument));
			argument.clear();
		}
	};

	while (true)
	{
		if (word == TEXT(")"))
		{
			flushArgument();
			co_yield true;
			break;
		}

		if (word == TEXT(","))
		{
			flushArgument();
			co_yield true;
		}

		argument.emplace_back(word);
		co_yield true;
	}

	if (word == TEXT("noexcept"))
	{
		ptr->bNoexcept = true;
		co_yield true;
	}

	if (word == TEXT("override"))
	{
		co_yield true;
	}

	if (word == TEXT(";"))
	{
		co_yield true;
		co_return;
	}

	if (word == TEXT("{"))
	{
		size_t step = 1;
		while (step > 0)
		{
			if (word == TEXT("{"))
			{
				++step;
			}
			else if (word == TEXT("}"))
			{
				--step;
			}

			co_yield true;
		}
	}

	co_yield true;
}

std::coroutine_handle<> CompiledSCLASS::STree_Field(CompiledSFIELD* ptr, const String& word, size_t& line)
{
	while (word != TEXT(";"))
	{
		co_yield true;
	}

	co_yield true;
}

std::coroutine_handle<> CompiledSCLASS::STree_Method(CompiledSMETHOD* ptr, const String& word, size_t& line)
{
	if (word != TEXT("("))
	{
		throw Exception(TEXTF("Syntax error: Unexpected identifier between function name and '('. Word: {}, Line: {}", word, line));
	}

	co_yield true;

	std::vector<String> argument;
	auto flushArgument = [&]()
	{
		if (!argument.empty())
		{
			ptr->Args.emplace_back(String::Join(TEXT(" "), argument));
			argument.clear();
		}
	};

	while (true)
	{
		if (word == TEXT(")"))
		{
			flushArgument();
			co_yield true;
			break;
		}

		if (word == TEXT(","))
		{
			flushArgument();
			co_yield true;
		}

		argument.emplace_back(word);
		co_yield true;
	}

	if (word == TEXT("const"))
	{
		ptr->bReadonly = true;
		co_yield true;
	}

	if (word == TEXT("noexcept"))
	{
		ptr->bNoexcept = true;
		co_yield true;
	}

	if (word == TEXT("override"))
	{
		co_yield true;
	}

	if (word == TEXT(";"))
	{
		co_yield true;
		co_return;
	}
	
	if (word == TEXT("{"))
	{
		size_t step = 1;
		co_yield true;
		while (step > 0)
		{
			if (word == TEXT("{"))
			{
				++step;
			}
			else if (word == TEXT("}"))
			{
				--step;
			}

			co_yield true;
		}
	}

	co_yield true;
}