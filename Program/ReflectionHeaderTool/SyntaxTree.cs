// Copyright 2020-2022 Aumoa.lib. All right reserved.

using DotNETUtilities;

using System.Collections;

namespace ReflectionHeaderTool;

internal class SyntaxTree : IEnumerable<SyntaxCore>
{
    private readonly FileReference _fr;

    public SyntaxTree(FileReference source)
    {
        _fr = source;
    }

    private delegate SyntaxCore? ParseSyntaxCode(ref int i, ref int line, string sourceCode);
    private ParseSyntaxCode? _compiler;
    private readonly List<SyntaxCore> _syntaxes = new();

    private static bool IsAllowedIdentifier(char ch, bool startsWith = false)
    {
        return (ch >= 'a') && (ch <= 'z')
            || (ch >= 'A') && (ch <= 'Z')
            || (ch == '_')
            || (startsWith && (ch >= '0') && (ch <= '9'));
    }

    public void Parse()
    {
        string sourceCode = _fr.ReadAllText().ReplaceLineEndings("\n");
        int line = 1;

#if PRINT_SYNTAX
        Console.Write("{0,3}. ", 1);
        ConsoleColor backup = Console.ForegroundColor;
#endif
        for (int i = 0; i < sourceCode.Length; ++i)
        {
            if (sourceCode.EqualsSpan(i, "///"))
            {
#if PRINT_SYNTAX
                Console.ForegroundColor = ConsoleColor.DarkGreen;
                Console.Write("///");
#endif
                i += 2;
                _compiler = ParseCommentDoc;
            }
            else if (sourceCode.EqualsSpan(i, "//"))
            {
#if PRINT_SYNTAX
                Console.ForegroundColor = ConsoleColor.Green;
                Console.Write("//");
#endif
                ++i;
                _compiler = ParseCommentLine;
            }
            else if (sourceCode[i] == '#')
            {
#if PRINT_SYNTAX
                Console.ForegroundColor = ConsoleColor.DarkGray;
                Console.Write("#");
#endif
                _compiler = ParsePreprocessor;
            }
            else if (sourceCode.EqualsSpan(i, "/*"))
            {
                ++i;
#if PRINT_SYNTAX
                Console.Write("/*");
#endif
                _compiler = ParseCommentSpan;
            }
            else if (sourceCode[i] == '\n')
            {
                ++line;
#if PRINT_SYNTAX
                Console.WriteLine();
                Console.Write("{0,3}. ", line);
#endif
            }
            else if (sourceCode[i] == '"')
            {
#if PRINT_SYNTAX
                Console.ForegroundColor = ConsoleColor.DarkRed;
                Console.Write('"');
#endif
                _compiler = ParseString;
            }
            else
            {
                // Parser
                SyntaxCore? syntax = _compiler?.Invoke(ref i, ref line, sourceCode);

                // Keyword
                syntax ??= ParseKeyword(ref i, ref line, sourceCode);

                // Operator
                syntax ??= ParseOperator(ref i, ref line, sourceCode);

                // Scope
                syntax ??= ParseScope(ref i, ref line, sourceCode);

                // Identifier
                if (syntax == null && IsAllowedIdentifier(sourceCode[i]))
                {
                    syntax = ParseIdentifier(ref i, ref line, sourceCode);
                }

                // Number
                if (syntax == null && char.IsNumber(sourceCode[i]))
                {
                    syntax = ParseNumber(ref i, ref line, sourceCode);
                }

                // Semicolon
                if (syntax == null && sourceCode[i] == ';')
                {
#if PRINT_SYNTAX
                    Console.Write(';');
#endif
                    syntax = new SyntaxCore(SyntaxType.Semicolon, ";", line);
                }

                if (syntax != null)
                {
                    _syntaxes.Add(syntax.Value);
                }

#if PRINT_SYNTAX
                if (_compiler == null && syntax == null)
                {
                    Console.Write(sourceCode[i]);
                }
                Console.ForegroundColor = backup;
#endif

                _compiler = null;
            }
        }

#if PRINT_SYNTAX
        Console.WriteLine("");
        Console.WriteLine("Parsed Syntaxes: ");
        foreach (var syntax in _syntaxes)
        {
            Console.WriteLine("SyntaxType: {0}, SyntaxName: {1}", syntax.Type, syntax.Name);
        }
#endif
    }

    private static SyntaxCore? ParseIdentifier(ref int i, ref int line, string sourceCode)
    {
        int start = i;
        bool isEof = true;
        bool isFirst = true;

#if PRINT_SYNTAX
        Console.ForegroundColor = ConsoleColor.Yellow;
#endif

        for (; i < sourceCode.Length; ++i)
        {
            if (!IsAllowedIdentifier(sourceCode[i], !isFirst))
            {
                --i;
                isEof = false;
                break;
            }
#if PRINT_SYNTAX
            else
            {
                Console.Write(sourceCode[i]);
            }
#endif
            isFirst = false;
        }

        return new SyntaxCore(SyntaxType.Identifier, sourceCode[start..(i + (isEof ? 0 : 1))], line);
    }

    public SyntaxCore? ParseNumber(ref int i, ref int line, string sourceCode)
    {
        string composed = "";
        bool containsDot = false;

        if (char.IsNumber(sourceCode[i]) || sourceCode[i] == '.')
        {
            for (; ; ++i)
            {
                if ((sourceCode[i] >= 'a' && sourceCode[i] <= 'z') || (sourceCode[i] >= 'A' && sourceCode[i] <= 'Z'))
                {
                    composed += sourceCode[i];
                    break;
                }
                else if (sourceCode[i] == '.')
                {
                    if (containsDot)
                    {
                        throw new CompilationException(CompilationException.ErrorCode.SyntaxError, "Duplicated dot on number.", _fr.FileName, line);
                    }
                    else
                    {
                        composed += '.';
                        containsDot = true;
                    }
                }
                else if (char.IsNumber(sourceCode[i]))
                {
                    composed += sourceCode[i];
                }
                else
                {
                    --i;
                    break;
                }
            }
        }

        return new SyntaxCore(SyntaxType.Number, composed, line);
    }

    private static SyntaxCore? ParsePreprocessor(ref int i, ref int line, string sourceCode)
    {
        int start = i;
        bool isEof = true;

        for (; i < sourceCode.Length; ++i)
        {
            if (sourceCode[i] == '\n' && sourceCode[i - 1] != '\\')
            {
                --i;
                isEof = false;
                break;
            }
#if PRINT_SYNTAX
            else
            {
                Console.Write(sourceCode[i]);
            }
#endif
        }

        return new SyntaxCore(SyntaxType.Preprocessor, sourceCode[start..(i + (isEof ? 0 : 1))], line);
    }

    private static SyntaxCore? ParseScope(ref int i, ref int line, string sourceCode)
    {
        char ch = sourceCode[i];
        if (ch == '{' || ch == '}')
        {
#if PRINT_SYNTAX
            Console.Write(ch);
#endif
            return new SyntaxCore(SyntaxType.Scope, ch.ToString(), line);
        }

        return null;
    }

    private static readonly string[] Operators = new[]
    {
        "(", ")", "[", "]", "<", ">",
        "&", "*", "->",
        "co_return", "co_await", "co_yield",
        "?", ":", ",",
        "+", "-", "/", "?", "^", "|", "=",
    };

    private static readonly HashSet<char> OperatorsIndex = Operators.Select(p => p[0]).ToHashSet();

    private static SyntaxCore? ParseOperator(ref int i, ref int line, string sourceCode)
    {
        if (OperatorsIndex.Contains(sourceCode[i]))
        {
            foreach (var op in Operators)
            {
                if (sourceCode.EqualsSpan(i, op))
                {
                    i += op.Length - 1;
#if PRINT_SYNTAX
                    Console.ForegroundColor = ConsoleColor.DarkGray;
                    Console.Write(op);
#endif
                    return new SyntaxCore(SyntaxType.Operator, op, line);
                }
            }
        }

        return null;
    }

    private static readonly string[] Keywords = new[]
    {
        "class", "struct", "interface", "enum",
        "public", "protected", "private",
        "virtual", "noexcept",
        "inline", "constexpr", "const", "static",
        "this",
        "using", "namespace", "typedef", "typename", "template",
        "return", "throw"
    };

    private static readonly HashSet<char> KeywordsIndex = Keywords.Select(p => p[0]).ToHashSet();

    private static SyntaxCore? ParseKeyword(ref int i, ref int line, string sourceCode)
    {
        if (KeywordsIndex.Contains(sourceCode[i]))
        {
            foreach (var keyword in Keywords)
            {
                if (sourceCode.EqualsSpan(i, keyword))
                {
                    i += keyword.Length - 1;
#if PRINT_SYNTAX
                    Console.ForegroundColor = ConsoleColor.Blue;
                    Console.Write(keyword);
#endif
                    return new SyntaxCore(SyntaxType.Keyword, keyword, line);
                }
            }
        }

        return null;
    }

    private static SyntaxCore? ParseString(ref int i, ref int line, string sourceCode)
    {
        int start = i;

        for (; i < sourceCode.Length; ++i)
        {
            if (sourceCode[i] == '"' && sourceCode[i - 1] != '\\')
            {
#if PRINT_SYNTAX
                Console.Write('"');
#endif
                break;
            }
#if PRINT_SYNTAX
            else
            {
                Console.Write(sourceCode[i]);
            }
#endif
        }

        return new SyntaxCore(SyntaxType.String, sourceCode[start..i], line);
    }

    private static SyntaxCore? ParseCommentLine(ref int i, ref int line, string sourceCode)
    {
        int start = i;
        bool isEof = true;

        for (; i < sourceCode.Length; ++i)
        {
            if (sourceCode[i] == '\n')
            {
                if (sourceCode[i - 1] != '\\')
                {
                    --i;
                    isEof = false;
                    break;
                }
                else
                {
                    ++line;
                }
            }
#if PRINT_SYNTAX
            else
            {
                Console.Write(sourceCode[i]);
            }
#endif
        }

        return new SyntaxCore(SyntaxType.Comment, sourceCode[start..(i + (isEof ? 0 : 1))], line);
    }

    private static SyntaxCore? ParseCommentDoc(ref int i, ref int line, string sourceCode)
    {
        int start = i;
        bool isEof = true;

        for (; i < sourceCode.Length; ++i)
        {
            if (sourceCode[i] == '\n')
            {
                if (sourceCode[i - 1] == '\\')
                {
                    ++line;
#if PRINT_SYNTAX
                    Console.WriteLine();
#endif
                    continue;
                }

                --i;
                isEof = false;
                break;
            }
#if PRINT_SYNTAX
            else
            {
                Console.Write(sourceCode[i]);
            }
#endif
        }

        return new SyntaxCore(SyntaxType.CommentDoc, sourceCode[start..(i + (isEof ? 0 : 1))], line);
    }

    private static SyntaxCore? ParseCommentSpan(ref int i, ref int line, string sourceCode)
    {
        int start = i;

        for (; i < sourceCode.Length; ++i)
        {
            if (sourceCode.EqualsSpan(i, "*/"))
            {
                ++i;
#if PRINT_SYNTAX
                Console.Write("*/");
#endif
                break;
            }
            else if (sourceCode[i] == '\n')
            {
#if PRINT_SYNTAX
                Console.WriteLine();
                Console.Write("{0,3}. ", line);
#endif
                ++line;
            }
#if PRINT_SYNTAX
            else
            {
                Console.Write(sourceCode[i]);
            }
#endif
        }

        return new SyntaxCore(SyntaxType.Comment, sourceCode[start..i], line);
    }

    public IEnumerator<SyntaxCore> GetEnumerator()
    {
        return _syntaxes.GetEnumerator();
    }

    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
}
