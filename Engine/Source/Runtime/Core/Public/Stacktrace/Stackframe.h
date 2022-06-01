// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <filesystem>

struct Stackframe
{
	String ModuleName;
	String FunctionName;
	String FileName;
	int32 Line;
	int64 AddressOf;

    String GetCleanedFileName(size_t allowedNestedLevel = 2) const noexcept
    {
        if (!FileName)
        {
            return String::Format(TEXT("0x{:0>16X}"), AddressOf);
        }

        auto p = std::filesystem::path((const std::wstring&)FileName);
        String cleaned = String(p.filename().wstring());

        for (size_t i = 0; i < allowedNestedLevel && p.has_parent_path(); ++i)
        {
            p = p.parent_path();
            cleaned = String::Format(TEXT("{}/{}"), p.stem().wstring(), cleaned);
        }

        if (p.has_parent_path())
        {
            cleaned = String(TEXT(".../")) + cleaned;
        }
        return cleaned;
    }

	String GetCleanedFunctionName(size_t allowNestedLevel = 2, bool fullNamespace = false) const noexcept
	{
        size_t templateStack = 0;

        struct ShrinkScope
        {
            size_t startOf;
            size_t endOf;
        };

        std::vector<ShrinkScope> scopes;
        std::optional<ShrinkScope> scope;

        for (size_t i = 0; i < (size_t)FunctionName; ++i)
        {
            if (FunctionName[i] == TEXT('<'))
            {
                ++templateStack;

                if (templateStack >= allowNestedLevel && !scope.has_value())
                {
                    scope.emplace().startOf = i + 1;
                }
            }
            else if (FunctionName[i] == TEXT('>'))
            {
                --templateStack;
                if (templateStack < allowNestedLevel && scope.has_value())
                {
                    scope->endOf = i;
                    scopes.emplace_back(*scope);
                    scope.reset();
                }
            }
        }

        String cleaned = FunctionName;
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
        {
            auto& scope = *it;
            size_t len = scope.endOf - scope.startOf;
            bool isLambda = String(cleaned).Contains(TEXT("lambda"), scope.startOf, len);
            if (isLambda)
            {
                cleaned = cleaned.ReplaceAt(scope.startOf, len, TEXT("lambda"));
            }
            else
            {
                cleaned = cleaned.ReplaceAt(scope.startOf, len, TEXT("..."));
            }
        }

        return _Cleanup_nested_namespaces(cleaned);
	}

private:
    static inline String _Cleanup_nested_namespaces(String nested) noexcept
    {
        std::vector<size_t> scopes;
        String result(nested);

        size_t lpos = 0;
        size_t tstack = 0;
        for (size_t i = 0; i < (size_t)result; ++i)
        {
            auto ch = result[i];
            if (ch == TEXT('<'))
            {
                if (tstack++ == 0)
                {
                    lpos = i + 1;
                }
            }
            else if (ch == TEXT('>'))
            {
                if (--tstack == 0)
                {
                    size_t len = i - lpos;
                    String tscope = _Cleanup_nested_namespaces(result.Substring(lpos, len));
                    result = result.ReplaceAt(lpos, len, tscope);
                    lpos += (size_t)tscope;
                    i = lpos;
                }
            }

            if (tstack == 0)
            {
                if (ch == TEXT(':') && i != 0 && result[i - 1] == TEXT(':'))
                {
                    scopes.emplace_back(i - 1);
                }
                else if (ch == TEXT(','))
                {
                    size_t startOf = i + 1;
                    size_t len = (size_t)result - startOf;
                    String commaSep = _Cleanup_nested_namespaces(result.Substring(startOf));
                    result = result.ReplaceAt(startOf, len, commaSep);
                    break;
                }
            }
        }

        if (scopes.size() >= 2)
        {
            size_t front = scopes.front();
            size_t mid = *(scopes.rbegin() + 1) + 2;
            size_t back = scopes.back();

            return String::Format(TEXT("{}::...::{}::{}"),
                result.Substring(0, front),
                result.Substring(mid, back - mid),
                result.Substring(back + 2)
            );
        }

        return result;
    }
};