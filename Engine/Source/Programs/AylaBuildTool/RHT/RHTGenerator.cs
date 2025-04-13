using System.Diagnostics.CodeAnalysis;
using Microsoft.CodeAnalysis;
using Spectre.Console;
using static System.Runtime.InteropServices.JavaScript.JSType;
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
        public readonly string BaseClass;

        private Class(Context context, string name, char? escapeBracket, Namespace[] namespaces, string @base) : base(context)
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
            BaseClass = @base;
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
            string @base = string.Empty;
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
                    var inheritanceStr = context.Export(0, "{");
                    var inheritances = inheritanceStr.ToString().Split(',', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                    @base = inheritances[0].Replace("public ", string.Empty);
                    escapeBracket = '}';
                    break;
                default:
                    throw context.ParsingError("Syntax Error: Expected '{' or ';' but found an unexpected character.");
            }

            @class = new Class(context, name.ToString(), escapeBracket, bracketStack.OfType<Namespace>().ToArray(), @base);
            return true;
        }
    }

    internal class AClass : Syntax
    {
        public readonly Class Class;
        private GeneratedBody? m_GeneratedBody;
        private List<AProperty> m_Properties = [];
        private List<AFunction> m_Functions = [];

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

        public IReadOnlyList<AFunction> Functions => m_Functions;

        public void AddProperty(AProperty property)
        {
            m_Properties.Add(property);
        }

        public void AddFunction(AFunction function)
        {
            m_Functions.Add(function);
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
            var nextLine = context.Export(0, ";").ToString();
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

    internal class AFunction : Syntax
    {
        public record ParameterInfo
        {
            public required string TypeName { get; init; }
            public required string Name { get; init; }
            public required string? DefaultValue { get; init; }
        }

        public readonly string Name;
        public readonly string Return;
        public readonly bool Static;
        public readonly bool Virtual;
        public readonly ParameterInfo[] ParameterInfos;

        private AFunction(Context context, string name, string @return, bool @static, bool @virtual, IEnumerable<ParameterInfo> parameterInfos) : base(context)
        {
            Name = name;
            Return = @return;
            Static = @static;
            Virtual = @virtual;
            ParameterInfos = parameterInfos.ToArray();
        }

        public override string ToString()
        {
            return FormatLineNumber() + "AFUNCTION()";
        }

        public override char? EscapeBracket => null;

        public static bool TryAccept(Context context, [NotNullWhen(true)] out AFunction? afunction)
        {
            if (context.WholeEquals("AFUNCTION()") == false)
            {
                afunction = null;
                return false;
            }

            var capture = context.Capture();
            context.Advance("AFUNCTION()".Length);

            context.SkipWhiteSpace(true);
            var nextLine = context.Export(0, "(").ToString();
            var items = nextLine.Split([' ', '\t', '\n'], StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
            int index = 0;
            int step = 0;
            bool @virtual = false, @static = false;
            string returnType = string.Empty;
            string name = string.Empty;
            AssertExpression("Syntax Error: A function implementation is required, but no implementation exists.");
            while (index < items.Length)
            {
                _ = TryAcceptStaticOrVirtual() ||
                    TryAcceptReturnType() ||
                    TryAcceptName();
            }

            var parametersStr = context.Export(0, ";").Trim([' ', '\t', '\n', '(', ')']).ToString();
            var parameters = parametersStr.Split(',', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
            List<ParameterInfo> parameterInfos = [];
            foreach (var parameter in parameters)
            {
                string? defaultValue = null;

                var defaultValueSplit = parameter.Split('=');
                if (defaultValueSplit.Length >= 2)
                {
                    defaultValue = defaultValueSplit[1].Trim();
                }

                var declare = defaultValueSplit[0].Split(' ', '\t', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                parameterInfos.Add(new ParameterInfo
                {
                    TypeName = declare[0],
                    Name = declare[1],
                    DefaultValue = defaultValue
                });
            }
            afunction = new AFunction(capture, name, returnType, @static, @virtual, parameterInfos);
            return true;

            bool TryAcceptStaticOrVirtual()
            {
                if (step != 0)
                {
                    return false;
                }

                if (items[index] == "virtual")
                {
                    @virtual = true;
                    ++index;
                    return true;
                }

                if (items[index] == "static")
                {
                    @static = true;
                    ++index;
                    return true;
                }

                return false;
            }

            bool TryAcceptReturnType()
            {
                if (step != 0)
                {
                    return false;
                }

                returnType = items[index++];
                ++step;
                return true;
            }

            bool TryAcceptName()
            {
                if (step != 1)
                {
                    return false;
                }

                name = items[index++];
                return true;
            }

            void AssertExpression(string message)
            {
                if (index >= items.Length)
                {
                    throw capture.ParsingError(message);
                }
            }
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

                if (aclass.Body != null)
                {
                    lineNumber = aclass.Body.LineNumber;

                    headerText += $"#define GENERATED_BODY__IMPL__{m_FileId}__{lineNumber} \\\n";
                    headerText += $"\tGENERATED_BODY__DEFAULT_BODY({aclass.Class.Name}) \\\n";
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

                string @namespace = aclass.Class.Namespace;
                string @class = aclass.Class.Name;

                sourceCodeText += $"ACLASS__IMPL_CLASS_REGISTER({@namespace}, {@class});\n";
                sourceCodeText +=  "\n";
                sourceCodeText +=  "extern \"C\"\n";
                sourceCodeText +=  "{\n";
                sourceCodeText += $"\tPLATFORM_SHARED_EXPORT ::Ayla::ssize_t {FunctionName1("New")}()\n";
                sourceCodeText +=  "\t{\n";
                sourceCodeText += $"\t\tauto new_ = ::Ayla::Object::UnsafeNew<::{@namespace}::{@class}>(0);\n";
                sourceCodeText += $"\t\tauto& self__ = *new ::Ayla::RPtr<::Ayla::Object>(new_);\n";
                sourceCodeText += $"\t\treturn ::Ayla::Marshal::RPtrToIntPtr(self__);\n";
                sourceCodeText +=  "\t}\n";
                sourceCodeText +=  "\n";
                sourceCodeText += $"\tPLATFORM_SHARED_EXPORT void {FunctionName1("Finalize")}(::Ayla::ssize_t self_)\n";
                sourceCodeText +=  "\t{\n";
                sourceCodeText += $"\t\tauto& self__ = ::Ayla::Marshal::IntPtrToRPtr(self_);\n";
                sourceCodeText += $"\t\tdelete &self__;\n";
                sourceCodeText +=  "\t}\n";
                sourceCodeText +=  "}\n";
                sourceCodeText +=  "\n";

                if (aclass.Body != null)
                {
                    sourceCodeText += $"namespace {@namespace}\n";
                    sourceCodeText +=  "{\n";
                    sourceCodeText += $"\tvoid {@class}::GatherProperties(::Ayla::PropertyCollector& collector)\n";
                    sourceCodeText +=  "\t{\n";
                    sourceCodeText +=  "\t\tSuper::GatherProperties(collector);\n";
                    foreach (var property in aclass.Properties)
                    {
                        sourceCodeText += $"\t\tGENERATED_BODY__GATHER_PROPERTIES_PROP({property.Name});\n";
                    }
                    sourceCodeText +=  "\t}\n";
                    sourceCodeText +=  "}\n\n";
                    sourceCodeText +=  "extern \"C\"\n";
                    sourceCodeText +=  "{\n";
                    foreach (var function in aclass.Functions)
                    {
                        List<string> parameters = [];
                        if (function.Static == false)
                        {
                            parameters.Add("::Ayla::ssize_t self_");
                        }

                        List<string> arguments = [];

                        sourceCodeText += $"\tPLATFORM_SHARED_EXPORT {FormatReturnType1()} {FunctionName1(function.Name)}({string.Join(", ", parameters)})\n";
                        sourceCodeText +=  "\t{\n";
                        if (function.Static == false)
                        {
                            sourceCodeText += $"\t\tauto& self__ = {Self__()};\n";
                        }
                        sourceCodeText += $"\t\treturn {CallFunction()};\n";
                        sourceCodeText +=  "\t}\n";

                        string CallFunction()
                        {
                            string prefix;
                            if (function.Static)
                            {
                                prefix = $"::{@namespace}::{@class}::";
                            }
                            else
                            {
                                prefix = $"self__->";
                            }

                            return $"{prefix}{function.Name}(" + string.Join(", ", arguments) + ")";
                        }

                        string FormatReturnType1() => FormatVariableType(function.Context, function.Return);
                    }
                    sourceCodeText += "}\n";
                    sourceCodeText += "\n";
                }

                string FunctionName1(string name) => FunctionName(@namespace, @class, name);

                string Self__()
                {
                    return $"(::Ayla::Marshal::IntPtrToRPtr<::{@namespace}::{@class}>(self_))";
                }
            }
        }

        return sourceCodeText;
    }

    public string GenerateBindings()
    {
        string sourceCodeText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

using System.Runtime.InteropServices;


""";

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is AClass aclass)
            {
                string @namespace = aclass.Class.Namespace;
                string @class = aclass.Class.Name;
                string inherit;
                if (string.IsNullOrEmpty(aclass.Class.BaseClass) == false)
                {
                    inherit = $" : {aclass.Class.BaseClass}";
                }
                else
                {
                    inherit = " : global::System.IDisposable";
                }

                sourceCodeText += $"namespace {@namespace}\n";
                sourceCodeText +=  "{\n";
                sourceCodeText += $"\tpublic class {@class}{inherit}\n";
                sourceCodeText +=  "\t{\n";
                if (string.IsNullOrEmpty(aclass.Class.BaseClass))
                {
                    sourceCodeText +=  "\t\tprivate nint m_InstancePtr;\n";
                    sourceCodeText +=  "\t\t\n";
                    sourceCodeText +=  "\t\tpublic long InstancePtr => m_InstancePtr;\n";
                    sourceCodeText +=  "\t\t\n";
                    sourceCodeText += $"\t\tpublic {@class}()\n";
                    sourceCodeText +=  "\t\t{\n";
                    sourceCodeText += $"\t\t\tm_InstancePtr = {FunctionName1("New")}();\n";
                    sourceCodeText +=  "\t\t}\n";
                    sourceCodeText +=  "\t\t\n";
                    sourceCodeText += $"\t\tprotected {@class}(nint instancePtr)\n";
                    sourceCodeText +=  "\t\t{\n";
                    sourceCodeText +=  "\t\t\tm_InstancePtr = instancePtr;\n";
                    sourceCodeText +=  "\t\t}\n";
                    sourceCodeText +=  "\t\t\n";
                    sourceCodeText += $"\t\t~{@class}()\n";
                    sourceCodeText +=  "\t\t{\n";
                    sourceCodeText +=  "\t\t\tDispose(false);\n";
                    sourceCodeText +=  "\t\t}\n";
                    sourceCodeText +=  "\t\t\n";
                    sourceCodeText +=  "\t\tpublic void Dispose()\n";
                    sourceCodeText +=  "\t\t{\n";
                    sourceCodeText +=  "\t\t\tDispose(true);\n";
                    sourceCodeText +=  "\t\t\tglobal::System.GC.SuppressFinalize(this);\n";
                    sourceCodeText +=  "\t\t}\n";
                    sourceCodeText +=  "\t\t\n";
                    sourceCodeText +=  "\t\tprotected virtual void Dispose(bool disposing)\n";
                    sourceCodeText +=  "\t\t{\n";
                    sourceCodeText += $"\t\t\t{FunctionName1("Finalize")}(m_InstancePtr);\n";
                    sourceCodeText +=  "\t\t\tm_InstancePtr = 0;\n";
                    sourceCodeText +=  "\t\t}\n";
                    sourceCodeText +=  "\t\t\n";
                }
                else
                {
                    sourceCodeText += $"\t\tpublic {@class}() : base({FunctionName1("New")}())\n";
                    sourceCodeText +=  "\t\t{\n";
                    sourceCodeText +=  "\t\t}\n";
                    sourceCodeText +=  "\t\t\n";
                    sourceCodeText += $"\t\tprotected {@class}(nint instancePtr) : base(instancePtr)\n";
                    sourceCodeText +=  "\t\t{\n";
                    sourceCodeText +=  "\t\t}\n";
                    sourceCodeText +=  "\t\t\n";
                }
                sourceCodeText += $"\t\t[DllImport(\"{m_SourceCode.ModuleName}\")]\n";
                sourceCodeText += $"\t\tprivate static extern nint {FunctionName1("New")}();\n";
                sourceCodeText +=  "\t\t\n";
                sourceCodeText += $"\t\t[DllImport(\"{m_SourceCode.ModuleName}\")]\n";
                sourceCodeText += $"\t\tprivate static extern void {FunctionName1("Finalize")}(nint self_);\n";
                foreach (var function in aclass.Functions)
                {
                    List<string> parameters = [];
                    List<string> arguments = [];

                    IEnumerable<string> parametersWithSelf = parameters;
                    IEnumerable<string> argumentsWithSelf = arguments;
                    string prefix = string.Empty;

                    foreach (var parameter in function.ParameterInfos)
                    {
                        parameters.Add($"{FormatVariableType(function.Context, parameter.TypeName)} {parameter.Name}{(parameter.DefaultValue == null ? string.Empty : $" = {parameter.DefaultValue}")}");
                        arguments.Add(parameter.Name);
                    }

                    if (function.Static == false)
                    {
                        parametersWithSelf = Enumerable.Repeat("nint self_", 1).Concat(parameters);
                        argumentsWithSelf = Enumerable.Repeat("(nint)InstancePtr", 1).Concat(arguments);
                    }
                    else
                    {
                        prefix += "static ";
                    }

                    if (function.Virtual)
                    {
                        prefix += "virtual ";
                    }

                    sourceCodeText +=  "\t\t\n";
                    sourceCodeText += $"\t\tpublic {prefix}{FormatReturnType1()} {function.Name}({string.Join(", ", parameters)})\n";
                    sourceCodeText +=  "\t\t{\n";
                    sourceCodeText += $"\t\t\t{ReturnSyntax()}{FunctionName1(function.Name)}({string.Join(", ", argumentsWithSelf)});\n";
                    sourceCodeText +=  "\t\t}\n";
                    sourceCodeText +=  "\t\t\n";
                    sourceCodeText +=  "\t\t\n";
                    sourceCodeText += $"\t\t[DllImport(\"{m_SourceCode.ModuleName}\")]\n";
                    sourceCodeText += $"\t\tprivate static extern {FormatReturnType1()} {FunctionName1(function.Name)}({string.Join(", ", parametersWithSelf)});\n";

                    string FormatReturnType1() => FormatVariableType(function.Context, function.Return);
                    string ReturnSyntax() => function.Return == "void" ? string.Empty : "return ";
                }
                sourceCodeText += "\t}\n";
                sourceCodeText +=  "}\n";
                sourceCodeText +=  "\n";

                string FunctionName1(string name) => FunctionName(@namespace, @class, name);
            }
        }

        return sourceCodeText;
    }

    private static string FunctionName(string @namespace, string @class, string name)
    {
        return $"Injected__{@namespace}__{@class}__{name}";
    }

    private static string FormatVariableType(Context context, string variableType)
    {
        return variableType switch
        {
            "void" => "void",
            "bool" => "bool",
            "::Ayla::int32" or "Ayla::int32" or "int32" => "int",
            "::Ayla::int64" or "Ayla::int64" or "int64" => "long",
            "::Ayla::ssize_t" or "Ayla::ssize_t" or "ssize_t" => "nint",
            "::Ayla::uint32" or "Ayla::uint32" or "uint32" => "uint",
            "::Ayla::uint64" or "Ayla::uint64" or "uint64" => "ulong",
            "::Ayla::string" or "Ayla::string" or "string" => "string",
            _ => throw context.ParsingError($"Syntax Error: Unsupported return type '{variableType}'.")
        };
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
            else if (AFunction.TryAccept(context, out var afunction))
            {
                aclass = syntaxes.OfType<AClass>().LastOrDefault();
                if (aclass == null)
                {
                    throw afunction.Context.ParsingError("Syntax Error: A class definition is required before AFUNCTION().");
                }
                aclass.AddFunction(afunction);
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
