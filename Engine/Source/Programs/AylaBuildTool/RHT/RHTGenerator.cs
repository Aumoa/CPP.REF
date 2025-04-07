using System.Diagnostics.CodeAnalysis;
using Microsoft.CodeAnalysis;
using Spectre.Console;
using static AylaEngine.RHTGenerator;

namespace AylaEngine;

internal class RHTGenerator
{
    private class ParsingErrorException : Exception
    {
        public ParsingErrorException(string filePath, int lineNumber, int columnNumber, ReadOnlySpan<char> message)
            : base($"{filePath}({lineNumber},{columnNumber}): {message}")
        {
        }
    }

    internal class Context
    {
        public readonly string FilePath;
        private readonly string m_FullText;
        private int m_Index;
        private bool m_Freezed;

        public int LineNumber { get; private set; } = 1;

        public int ColumnNumber { get; private set; } = 0;

        private Context(string filePath, string fullText, int index, bool freezed, int lineNumber, int columnNumber)
        {
            FilePath = filePath;
            m_FullText = fullText;
            m_Index = index;
            m_Freezed = freezed;
            LineNumber = lineNumber;
            ColumnNumber = columnNumber;
        }

        public Context(string filePath, string fullText)
        {
            FilePath = filePath;
            m_FullText = fullText;
        }

        public Context Capture()
        {
            return new Context(FilePath, m_FullText, m_Index, true, LineNumber, ColumnNumber);
        }

        public ReadOnlySpan<char> Current => m_FullText.AsSpan(m_Index);

        public char CurrentChar => m_Index < m_FullText.Length ? m_FullText[m_Index] : '\0';

        public bool IsEOF => m_Index >= m_FullText.Length;

        public bool WholeEquals(ReadOnlySpan<char> value)
        {
            return Current.StartsWith(value) && (m_Index + value.Length >= m_FullText.Length || char.IsWhiteSpace(m_FullText[m_Index + value.Length]));
        }

        public bool Advance(int advance)
        {
            if (m_Freezed)
            {
                throw TerminateException.Internal();
            }

            for (int i = m_Index; i < m_FullText.Length && advance > 0;)
            {
                --advance;
                if (++m_Index >= m_FullText.Length)
                {
                    return false;
                }

                var ch = m_FullText[m_Index];
                if (ch == '\n')
                {
                    ColumnNumber = 0;
                    ++LineNumber;
                }
                else if (ch == '\r')
                {
                    continue;
                }
                else
                {
                    ++ColumnNumber;
                }
            }

            return true;
        }

        public bool WholeAdvance(ReadOnlySpan<char> value)
        {
            Advance(value.Length);
            for (; IsEOF == false;)
            {
                if (char.IsWhiteSpace(CurrentChar))
                {
                    Advance(1);
                }
                else
                {
                    break;
                }
            }

            return IsEOF == false;
        }

        public void SkipWhiteSpace(bool allowNewLine)
        {
            while (m_Index < m_FullText.Length)
            {
                var ch = m_FullText[m_Index];
                if (char.IsWhiteSpace(ch) && (allowNewLine || (ch != '\n' && ch != '\r')))
                {
                    Advance(1);
                    continue;
                }

                break;
            }
        }

        public ReadOnlySpan<char> Export(int enter, ReadOnlySpan<char> escape)
        {
            int start = m_Index + enter;
            for (int i = start; i < m_FullText.Length; ++i)
            {
                if (m_FullText.AsSpan(i).StartsWith(escape))
                {
                    var result = m_FullText.AsSpan(start, i - start);
                    Advance(result.Length + escape.Length);
                    return ReplaceNewLine(result);
                }
            }

            throw ParsingError("Syntax Error: Expected closing escape character.");
        }

        public int SelectExport(int enter, params string[] values)
        {
            int start = m_Index + enter;
            for (int i = start; i < m_FullText.Length; ++i)
            {
                for (int j = 0; j < values.Length; ++j)
                {
                    if (m_FullText.AsSpan(i).StartsWith(values[j]))
                    {
                        return j;
                    }
                }
            }

            return -1;
        }

        public Exception ParsingError(ReadOnlySpan<char> message)
        {
            return new ParsingErrorException(FilePath, LineNumber, ColumnNumber, message);
        }

        private static ReadOnlySpan<char> ReplaceNewLine(ReadOnlySpan<char> source)
        {
            if (source[0] == '\n')
            {
                source = source[1..];
            }
            
            if (source[^1] == '\r')
            {
                source = source[..^1];
            }

            if (source.Contains("\r\n", StringComparison.Ordinal))
            {
                source = source.ToString().Replace("\r\n", "\n").AsSpan();
            }

            return source;
        }
    }

    internal abstract class Syntax
    {
        public readonly Context Context;

        public int LineNumber => Context.LineNumber;

        public int ColumnNumber => Context.ColumnNumber;

        protected Syntax(Context context)
        {
            Context = context;
        }

        public abstract char? EscapeBracket { get; }

        protected string FormatLineNumber()
        {
            return $"({LineNumber},{ColumnNumber}): ";
        }
    }

    internal class Include : Syntax
    {
        public readonly string Path;

        private Include(Context context, ReadOnlySpan<char> path) : base(context)
        {
            Path = path.ToString();
        }

        public override string ToString()
        {
            return FormatLineNumber() + $"#include \"{Path}\"";
        }

        public override char? EscapeBracket => null;

        public static bool TryAccept(Context context, [NotNullWhen(true)] out Include? include)
        {
            if (context.Current.StartsWith("#include") == false)
            {
                include = null;
                return false;
            }

            var capture = context.Capture();
            context.Advance("#include".Length);
            context.SkipWhiteSpace(false);

            ReadOnlySpan<char> escape;
            switch (context.CurrentChar)
            {
                case '<':
                    escape = ">";
                    break;
                case '\"':
                    escape = "\"";
                    break;
                default:
                    throw capture.ParsingError("Syntax Error: Expected '<' or '\"' but found an unexpected character.");
            }

            var includePath = context.Export(1, escape).Trim();
            include = new Include(capture, includePath);
            return true;
        }
    }

    internal class Comment : Syntax
    {
        public readonly string Text;

        private Comment(Context context, string text) : base(context)
        {
            Text = text;
        }

        public override string ToString()
        {
            return FormatLineNumber() + $"// {Text}";
        }

        public override char? EscapeBracket => null;

        public static bool TryAccept(Context context, [NotNullWhen(true)] out Comment? comment)
        {
            var capture = context.Capture();
            if (context.Current.StartsWith("//"))
            {
                var text = context.Export(2, "\n").Trim();
                comment = new Comment(capture, text.ToString());
                return true;
            }
            else if (context.Current.StartsWith("/*"))
            {
                var text = context.Export(2, "*/").Trim();
                comment = new Comment(capture, text.ToString());
                return true;
            }

            comment = null;
            return false;
        }
    }

    internal class Namespace : Syntax
    {
        public readonly string Name;

        private Namespace(Context context, string name) : base(context)
        {
            Name = name;
        }

        public override string ToString()
        {
            return FormatLineNumber() + $"namespace {Name}";
        }

        public override char? EscapeBracket => '}';

        public static bool TryAccept(Context context, [NotNullWhen(true)] out Namespace? @namespace)
        {
            if (context.WholeEquals("namespace") == false)
            {
                @namespace = null;
                return false;
            }

            var capture = context.Capture();
            context.WholeAdvance("namespace");
            context.SkipWhiteSpace(true);
            var name = context.Export(0, "{").Trim();
            @namespace = new Namespace(capture, name.ToString());
            return true;
        }
    }

    internal class Class : Syntax
    {
        public readonly string Name;
        public readonly string? DllSpec;
        private char? m_EscapeBracket;
        private Namespace[] m_Namespaces;

        private Class(Context context, string name, char? escapeBracket, Namespace[] namespaces) : base(context)
        {
            var values = name.Split(' ', '\n', '\t', '\r');
            if (values.Length >= 2)
            {
                DllSpec = values[0];
                Name = values[1];
            }
            else
            {
                DllSpec = null;
                Name = name;
            }
            m_EscapeBracket = escapeBracket;
            m_Namespaces = namespaces;
        }

        public override string ToString()
        {
            if (DllSpec == null)
            {
                return FormatLineNumber() + $"class {Name}";
            }
            else
            {
                return FormatLineNumber() + $"class {DllSpec} {Name}";
            }
        }

        public override char? EscapeBracket => m_EscapeBracket;

        public string Namespace => string.Join("::", m_Namespaces.Select(p => p.Name));

        public static bool TryAccept(Context context, List<Syntax> bracketStack, [NotNullWhen(true)] out Class? @class)
        {
            if (context.WholeEquals("class") == false)
            {
                @class = null;
                return false;
            }

            context.WholeAdvance("class");
            context.SkipWhiteSpace(true);
            ReadOnlySpan<char> name;
            Context capture = context.Capture();
            char? escapeBracket = null;
            switch (context.SelectExport(0, "{", ";", ":"))
            {
                case 0:
                    name = context.Export(0, "{").Trim();
                    escapeBracket = '}';
                    break;
                case 1:
                    name = context.Export(0, ";").Trim();
                    break;
                case 2:
                    name = context.Export(0, ":").Trim();
                    context.Export(0, "{");
                    escapeBracket = '}';
                    break;
                default:
                    throw context.ParsingError("Syntax Error: Expected '{' or ';' but found an unexpected character.");
            }

            @class = new Class(context, name.ToString(), escapeBracket, bracketStack.OfType<Namespace>().ToArray());
            return true;
        }
    }

    internal class AClass : Syntax
    {
        public readonly Class Class;
        private GeneratedBody? m_GeneratedBody;
        private List<AProperty> m_Properties = [];

        private AClass(Context context, Class @class) : base(context)
        {
            Class = @class;
        }

        public override string ToString()
        {
            return FormatLineNumber() + "ACLASS()";
        }

        public override char? EscapeBracket => Class.EscapeBracket;

        public GeneratedBody? Body
        {
            get => m_GeneratedBody;
            set
            {
                if (m_GeneratedBody != null)
                {
                    throw TerminateException.User();
                }

                m_GeneratedBody = value;
            }
        }

        public IReadOnlyList<AProperty> Properties => m_Properties;

        public void AddProperty(AProperty property)
        {
            m_Properties.Add(property);
        }

        public static bool TryAccept(Context context, List<Syntax> bracketStack, [NotNullWhen(true)] out AClass? aclass)
        {
            if (context.WholeEquals("ACLASS()") == false)
            {
                aclass = null;
                return false;
            }

            var capture = context.Capture();
            context.Advance("ACLASS()".Length);

            context.SkipWhiteSpace(true);

            if (Class.TryAccept(context, bracketStack, out var @class) == false)
            {
                throw context.ParsingError("A class definition is required after ACLASS().");
            }

            aclass = new AClass(capture, @class);
            return true;
        }
    }

    internal class GeneratedBody : Syntax
    {
        private GeneratedBody(Context context) : base(context)
        {
        }

        public override char? EscapeBracket => null;

        public override string ToString()
        {
            return FormatLineNumber() + "GENERATED_BODY()";
        }

        public static bool TryAccept(Context context, [NotNullWhen(true)] out GeneratedBody? generatedBody)
        {
            if (context.WholeEquals("GENERATED_BODY()") == false)
            {
                generatedBody = null;
                return false;
            }

            var capture = context.Capture();
            context.Advance("GENERATED_BODY()".Length);
            generatedBody = new GeneratedBody(capture);
            return true;
        }
    }

    internal class AProperty : Syntax
    {
        public readonly string Name;

        private AProperty(Context context, string name) : base(context)
        {
            Name = name;
        }

        public override string ToString()
        {
            return FormatLineNumber() + "APROPERTY()";
        }

        public override char? EscapeBracket => null;

        public static bool TryAccept(Context context, [NotNullWhen(true)] out AProperty? aproperty)
        {
            if (context.WholeEquals("APROPERTY()") == false)
            {
                aproperty = null;
                return false;
            }

            var capture = context.Capture();
            context.Advance("APROPERTY()".Length);

            context.SkipWhiteSpace(true);
            var nextLine = context.Export(0, "\n").Trim(';').ToString();
            var items = nextLine.Split('=');
            var declare = items[0].Split([' ', '\t'], StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
            if (declare.Length != 2)
            {
                throw capture.ParsingError("Syntax Error: Expected 'Type Name[ = DefaultValue];' but found an unexpected character.");
            }

            aproperty = new AProperty(capture, declare[1]);
            return true;
        }
    }

    internal class UnknownBracket : Syntax
    {
        private char m_EscapeBracket;

        private UnknownBracket(Context context, char escapeBracket) : base(context)
        {
            m_EscapeBracket = escapeBracket;
        }

        public override char? EscapeBracket => m_EscapeBracket;

        public override string ToString()
        {
            return FormatLineNumber() + m_EscapeBracket switch
            {
                '}' => "{}",
                ']' => "[]",
                ')' => "()",
                _ => throw TerminateException.Internal()
            };
        }

        public static bool TryAccept(Context context, [NotNullWhen(true)] out UnknownBracket? unknownBracket)
        {
            if (context.CurrentChar == '{' || context.CurrentChar == '[' || context.CurrentChar == '(')
            {
                var capture = context.Capture();
                unknownBracket = new UnknownBracket(capture, context.CurrentChar switch
                {
                    '{' => '}',
                    '[' => ']',
                    '(' => ')',
                    _ => throw new InvalidOperationException()
                });

                context.Advance(1);
                return true;
            }
            unknownBracket = null;
            return false;
        }
    }

    private readonly SourceCodeDescriptor m_SourceCode;
    private readonly Syntax[] m_Syntaxes;
    private readonly string m_FileId;

    private RHTGenerator(SourceCodeDescriptor sourceCode, Syntax[] syntaxes)
    {
        m_SourceCode = sourceCode;
        m_Syntaxes = syntaxes;
        var sourceRelativePath = Path.GetRelativePath(m_SourceCode.Group.SourceDirectory, m_SourceCode.FilePath);
        m_FileId = sourceRelativePath
            .Replace('/', '_')
            .Replace('\\', '_')
            .Replace('.', '_')
            .ToUpper();
    }

    public string GenerateHeader()
    {
        string headerText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#pragma once

#include "Reflection/ReflectionMacros.h"

#undef GENERATED_BODY__FILE_ID__
#define GENERATED_BODY__FILE_ID__ {m_FileId}


""";

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is AClass aclass)
            {
                var lineNumber = aclass.LineNumber;

                headerText += $"#define ACLASS__IMPL__{m_FileId}__{lineNumber}__NAMESPACE {aclass.Class.Namespace}\n";
                var classNameDefine = $"ACLASS__IMPL__{m_FileId}__{lineNumber}__CLASSNAME";
                headerText += $"#define {classNameDefine} {aclass.Class.Name}\n\n";

                if (aclass.Body != null)
                {
                    lineNumber = aclass.Body.LineNumber;

                    headerText += $"#define GENERATED_BODY__IMPL__{m_FileId}__{lineNumber} \\\n";
                    headerText += $"\tGENERATED_BODY__DEFAULT_BODY({classNameDefine}) \\\n";
                    headerText += $"\tGENERATED_BODY__DECLARE_GATHER_PROPERTIES()\n\n";
                }
            }
        }

        return headerText;
    }

    public string GenerateSourceCode()
    {
        string sourceCodeText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#include "CoreMinimal.h"
#include "{m_SourceCode.FilePath.Replace('\\', '/')}"


""";

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is AClass aclass)
            {
                var lineNumber = aclass.LineNumber;
                string namespaceDefine = $"ACLASS__IMPL__{m_FileId}__{lineNumber}__NAMESPACE";
                string classNameDefine = $"ACLASS__IMPL__{m_FileId}__{lineNumber}__CLASSNAME";

                sourceCodeText += $"ACLASS__IMPL_CLASS_REGISTER_2(\n";
                sourceCodeText += $"\t{namespaceDefine},\n";
                sourceCodeText += $"\t{classNameDefine}\n";
                sourceCodeText += $");\n\n";

                sourceCodeText +=  "extern \"C\"\n";
                sourceCodeText +=  "{\n";
                sourceCodeText += $"\tPLATFORM_SHARED_EXPORT ::Ayla::ssize_t ACLASS__DEFINE_BINDING_FUNCTION(\n";
                sourceCodeText += $"\t\t{namespaceDefine},\n";
                sourceCodeText += $"\t\t{classNameDefine},\n";
                sourceCodeText +=  "\t\tNew\n";
                sourceCodeText +=  "\t)()\n";
                sourceCodeText +=  "\t{\n";
                sourceCodeText += $"\t\tACLASS__NEW_FUNCTION_BODY({namespaceDefine}, {classNameDefine});\n";
                sourceCodeText +=  "\t}\n";
                sourceCodeText +=  "\n";
                sourceCodeText += $"\tPLATFORM_SHARED_EXPORT void ACLASS__DEFINE_BINDING_FUNCTION(\n";
                sourceCodeText += $"\t\t{namespaceDefine},\n";
                sourceCodeText += $"\t\t{classNameDefine},\n";
                sourceCodeText +=  "\t\tFinalize\n";
                sourceCodeText +=  "\t)(::Ayla::ssize_t self_)\n";
                sourceCodeText +=  "\t{\n";
                sourceCodeText += $"\t\tdelete reinterpret_cast<::Ayla::RPtr<::Ayla::Object>*>(self_);\n";
                sourceCodeText +=  "\t}\n";
                sourceCodeText +=  "}\n";
                sourceCodeText +=  "\n";

                if (aclass.Body != null)
                {
                    sourceCodeText += $"namespace {namespaceDefine}\n";
                    sourceCodeText +=  "{\n";
                    sourceCodeText += $"\tvoid {classNameDefine}::GatherProperties(::Ayla::PropertyCollector& collector)\n";
                    sourceCodeText +=  "\t{\n";
                    sourceCodeText +=  "\t\tSuper::GatherProperties(collector);\n";
                    foreach (var property in aclass.Properties)
                    {
                        sourceCodeText += $"\t\tGENERATED_BODY__GATHER_PROPERTIES_PROP({property.Name});\n";
                    }
                    sourceCodeText +=  "\t}\n";
                    sourceCodeText +=  "}\n\n";
                }
            }
        }

        return sourceCodeText;
    }

    public static async Task<RHTGenerator?> ParseAsync(SourceCodeDescriptor sourceCode, CancellationToken cancellationToken = default)
    {
        var headerFileName = Path.GetFileNameWithoutExtension(sourceCode.FilePath);
        var headerText = await File.ReadAllTextAsync(sourceCode.FilePath, cancellationToken);
        var includeText = $"#include \"{headerFileName}.gen.h\"";
        int includeIndex = headerText.IndexOf(includeText);
        if (includeIndex == -1)
        {
            return null;
        }

        // #include must be lowercase.
        if (headerText.AsSpan(includeIndex, "#include".Length).Equals("#include", StringComparison.Ordinal) == false)
        {
            return null;
        }

        List<Syntax> syntaxes = [];
        List<Syntax> bracketStack = [];

        var context = new Context(sourceCode.FilePath, headerText);
        for (; context.IsEOF == false;)
        {
            if (bracketStack.LastOrDefault()?.EscapeBracket == context.CurrentChar)
            {
                bracketStack.RemoveAt(bracketStack.Count - 1);
                context.Advance(1);
                continue;
            }

            if (Comment.TryAccept(context, out var comment))
            {
                syntaxes.Add(comment);
            }
            else if (Include.TryAccept(context, out var include))
            {
                syntaxes.Add(include);
            }
            else if (Namespace.TryAccept(context, out var @namespace))
            {
                syntaxes.Add(@namespace);
            }
            else if (Class.TryAccept(context, bracketStack, out var @class))
            {
                syntaxes.Add(@class);
            }
            else if (AClass.TryAccept(context, bracketStack, out var aclass))
            {
                syntaxes.Add(aclass);
            }
            else if (GeneratedBody.TryAccept(context, out var generatedBody))
            {
                aclass = syntaxes.OfType<AClass>().LastOrDefault();
                if (aclass == null)
                {
                    throw generatedBody.Context.ParsingError("Syntax Error: A class definition is required before GENERATED_BODY().");
                }
                aclass.Body = generatedBody;
                continue;
            }
            else if (AProperty.TryAccept(context, out var aproperty))
            {
                aclass = syntaxes.OfType<AClass>().LastOrDefault();
                if (aclass == null)
                {
                    throw aproperty.Context.ParsingError("Syntax Error: A class definition is required before APROPERTY().");
                }
                aclass.AddProperty(aproperty);
                continue;
            }
            else if (UnknownBracket.TryAccept(context, out var unknownBracket))
            {
                syntaxes.Add(unknownBracket);
            }
            else
            {
                context.Advance(1);
                continue;
            }

            if (syntaxes.Last().EscapeBracket.HasValue)
            {
                bracketStack.Add(syntaxes.Last());
            }
        }

        if (bracketStack.Count > 0)
        {
            throw bracketStack.Last().Context.ParsingError("Syntax Error: Unmatched brackets.");
        }

        var lastInclude = syntaxes.LastOrDefault(p => p is Include);
        if (lastInclude == null)
        {
            context.ParsingError("The mandatory header file must be included at the very end of the inclusion list.");
        }

        return new RHTGenerator(sourceCode, syntaxes.ToArray());
    }
}
