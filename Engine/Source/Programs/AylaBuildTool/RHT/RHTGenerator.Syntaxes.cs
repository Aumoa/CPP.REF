// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public class Include : Syntax
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

    public class Comment : Syntax
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

    public class Namespace : Syntax
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

    public class Class : Syntax
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

    public class AClass : Syntax
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

    public class GeneratedBody : Syntax
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

    public class AProperty : Syntax
    {
        public readonly TypeName TypeName;
        public readonly string Name;

        private AProperty(Context context, TypeName typeName, string name) : base(context)
        {
            TypeName = typeName;
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

            aproperty = new AProperty(capture, TypeName.FromNative(capture, declare[0]), declare[1]);
            return true;
        }
    }

    public class AFunction : Syntax
    {
        public record ParameterInfo
        {
            public required TypeName TypeName { get; init; }
            public required string Name { get; init; }
            public required string? DefaultValue { get; init; }
        }

        public readonly string Name;
        public readonly TypeName Return;
        public readonly bool Static;
        public readonly bool Virtual;
        public readonly ParameterInfo[] ParameterInfos;

        private AFunction(Context context, string name, TypeName @return, bool @static, bool @virtual, IEnumerable<ParameterInfo> parameterInfos) : base(context)
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
                    TypeName = TypeName.FromNative(capture, declare[0]),
                    Name = declare[1],
                    DefaultValue = defaultValue
                });
            }
            afunction = new AFunction(capture, name, TypeName.FromNative(capture, returnType), @static, @virtual, parameterInfos);
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

    public class UnknownBracket : Syntax
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
}
