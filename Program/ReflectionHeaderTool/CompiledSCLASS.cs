// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Text;

namespace ReflectionHeaderTool;

[DebuggerDisplay("SCLASS {_name}({_line})")]
internal class CompiledSCLASS : IHeaderGenerator
{
    private int _line;
    private string _name = null!;
    private string _apikey = null!;
    private string? _inherit;
    private readonly List<string> _interfaces = new();
    private int _generatedBody;
    private List<SyntaxConstructor> _constructors = new();
    private List<SyntaxProperty> _properties = new();

    public CompiledSCLASS()
    {
    }

    public void Compile(IEnumerator<SyntaxCore> generator, List<SyntaxCore> syntaxes)
    {
        _line = syntaxes.Last().Line;

        // Reading SCLASS attributes.
        ParseAttributes(generator);

        // Reading class key.
        var span = generator.EnumerateNext(sCore => sCore.Type == SyntaxType.Keyword, true);
        if (span == null)
        {
            throw new CompilationException(CompilationException.ErrorCode.InvalidClassKey, "Cannot find class key for SCLASS.", _line);
        }

        if (span[0].Name != "class")
        {
            throw new CompilationException(CompilationException.ErrorCode.InvalidClassKey, "Class key is not \"class\".", _line);
        }

        span = generator.EnumerateNext(sCore => sCore.Type == SyntaxType.Scope, true);
        if (span == null)
        {
            throw new CompilationException(CompilationException.ErrorCode.SyntaxError, "Class name does not specified.", _line);
        }

        ParseClassDeclaration(span.GetEnumerator());

        List<SyntaxCore> classBody = new();
        int scopeLevel = 1;
        for (; ;)
        {
            var value = generator.Current;
            if (value.Type == SyntaxType.Scope)
            {
                if (value.Name == "{")
                {
                    ++scopeLevel;
                }
                else if (value.Name == "}")
                {
                    if (--scopeLevel == 0)
                    {
                        classBody.Add(value);
                        break;
                    }
                }
            }

            classBody.Add(value);
            if (!generator.MoveNext())
            {
                break;
            }
        }

        ParseClassBody(classBody.GetEnumerator());
    }

    public virtual void GenerateHeader(string fileKey, StringBuilder sb)
    {
        // Summary
        sb.AppendLine($"// SCLASS for {_name}");
        sb.AppendLine();

        // Supports SCLASS()
        sb.AppendLine($"#define __LIBTY_SCLASS__{fileKey}__{_line}__ \\");
        sb.AppendLine($"__SCLASS_BEGIN_NAMESPACE()\\");
        sb.AppendLine($"\\");

        foreach (var ctor in _constructors)
        {
            sb.AppendLine($"__SCLASS_DECLARE_CONSTRUCTOR_INFO({_apikey}, {_name}, {ctor.SafeName})\\");
        }

        sb.AppendLine($"\\");

        foreach (var prop in _properties)
        {
            sb.AppendLine($"__SCLASS_DECLARE_PROPERTY_INFO({_name}, {prop.Variable.Name}, {prop.Variable.DefaultValue}, {prop.AccessModifier})\\");
        }

        sb.AppendLine($"\\");
        sb.AppendLine($"__SCLASS_DECLARE_REFLEXPR({_name})\\");

        sb.AppendLine($"\\");
        sb.AppendLine($"__SCLASS_END_NAMESPACE()");
        sb.AppendLine();

        // Supports GENERATED_BODY()
        sb.AppendLine($"#define __LIBTY_GENERATED_BODY__{fileKey}__{_generatedBody}__ \\");
        sb.AppendLine($"__SCLASS_DECLARE_GENERATED_BODY({_name}, {_inherit ?? "void"})");
        sb.AppendLine();
    }

    public virtual void GenerateSource(string fileKey, StringBuilder sb)
    {
        static string ComposeMacroArray(string type, List<string> strs)
        {
            if (strs.Count == 0)
            {
                return $"(std::vector<{type}>())";
            }

            return $"(std::vector<{type}>\r\n{{\r\n\t{string.Join(",\r\n\t", strs)}\r\n}})";
        }

        // Summary.
        sb.AppendLine($"// SCLASS for {_name}");
        sb.AppendLine();

        // Impl_GetType()
        sb.AppendLine($"__SCLASS_DEFINE_GENERATED_BODY({_name}, {_inherit});");
        sb.AppendLine();

        // Constructors.
        List<string> ctors = new();
        sb.AppendLine($"__SCLASS_BEGIN_NAMESPACE()");
        sb.AppendLine();

        foreach (var ctor in _constructors)
        {
            sb.AppendLine($"__SCLASS_DEFINE_CONSTRUCTOR_INFO({_name}, {ctor.SafeName}, ({ctor.ComposeInvokeArgs()}));");
            ctors.Add($"constructor_t(&Invoke_constructor__{_name}__{ctor.SafeName}__, {ctor.ComposeTypeHash()})");
        }

        sb.AppendLine();
        sb.AppendLine($"__SCLASS_DEFINE_REFLEXPR({_name}, {ComposeMacroArray("constructor_t", ctors)});");

        sb.AppendLine();
        sb.AppendLine($"__SCLASS_END_NAMESPACE()");
        sb.AppendLine();
    }

    private AccessModifier _currentAccessModifier = AccessModifier.Private;

    private void ParseClassBody(IEnumerator<SyntaxCore> syntaxes)
    {
        if (!syntaxes.MoveNext())
        {
            throw new CompilationException(CompilationException.ErrorCode.RuleError, "SCLASS must be contains GENERATED_BODY in first line.", _line);
        }

        var value = syntaxes.Current;
        if (value.Type != SyntaxType.Identifier && value.Name != "GENERATED_BODY")
        {
            throw new CompilationException(CompilationException.ErrorCode.RuleError, "SCLASS must be contains GENERATED_BODY in first line.", _line);
        }

        _generatedBody = value.Line;
        ParseAttributes(syntaxes);

        int scopeLevel = 0;
        for (; ; )
        {
            value = syntaxes.Current;

            if (value.Type == SyntaxType.Scope)
            {
                if (value.Name == "}")
                {
                    --scopeLevel;
                }
                else
                {
                    ++scopeLevel;
                }
            }

            if (value.Type == SyntaxType.Keyword && scopeLevel == 0)
            {
                if (value.Name == "public")
                {
                    _currentAccessModifier = AccessModifier.Public;
                }
                else if (value.Name == "protected")
                {
                    _currentAccessModifier = AccessModifier.Protected;
                }
                else if (value.Name == "private")
                {
                    _currentAccessModifier = AccessModifier.Private;
                }
            }

            if (scopeLevel == 0)
            {
                if (value.Type == SyntaxType.Identifier)
                {
                    if (value.Name == "SCONSTRUCTOR")
                    {
                        ParseConstructor(syntaxes);
                    }
                    else if (value.Name == "SPROPERTY")
                    {
                        ParseProperty(syntaxes);
                        continue;
                    }
                }
            }

            if (!syntaxes.MoveNext())
            {
                break;
            }
        }
    }

    private void ParseConstructor(IEnumerator<SyntaxCore> syntaxes)
    {
        int line = syntaxes.Current.Line;

        syntaxes.MoveNext();
        ParseAttributes(syntaxes);

        List<SyntaxCore>? prefix = syntaxes.EnumerateNext(sCore => sCore.Type == SyntaxType.Identifier && sCore.Name == _name, true);
        if (prefix == null)
        {
            throw new CompilationException(CompilationException.ErrorCode.RuleError, "Cannot find allowed constructor function for SCONSTRUCTOR().", line);
        }

        _constructors.Add(new()
        {
            Arguments = ParseArguments(syntaxes, line)
        });
    }

    private void ParseProperty(IEnumerator<SyntaxCore> syntaxes)
    {
        int line = syntaxes.Current.Line;

        syntaxes.MoveNext();
        ParseAttributes(syntaxes);

        List<SyntaxCore>? syntaxesVar = syntaxes.EnumerateNext(p => p.Type == SyntaxType.Semicolon);
        if (syntaxesVar == null)
        {
            throw new CompilationException(CompilationException.ErrorCode.SyntaxError, "Cannot find semicolon(;) after variable declaration.", line);
        }

        syntaxesVar.RemoveAt(syntaxesVar.Count - 1);
        SyntaxVariable sVar = SyntaxVariable.CreateFromSyntaxes(syntaxesVar);
        _properties.Add(new SyntaxProperty()
        {
            AccessModifier = _currentAccessModifier,
            Variable = sVar
        });
    }

    private static List<SyntaxVariable> ParseArguments(IEnumerator<SyntaxCore> syntaxes, int line)
    {
        List<SyntaxCore>? arguments = syntaxes.EnumerateNext(sCore => sCore.Type == SyntaxType.Operator && sCore.Name == ")", true);
        if (arguments == null)
        {
            throw new CompilationException(CompilationException.ErrorCode.SyntaxError, "Cannot find arguments list of constructor function.", line);
        }

        if (arguments[0].Type != SyntaxType.Operator || arguments[0].Name != "(")
        {
            throw new CompilationException(CompilationException.ErrorCode.SyntaxError, "Unexpected syntax before '('.", line);
        }

        arguments.RemoveAt(0);
        arguments.RemoveAt(arguments.Count - 1);

        List<SyntaxVariable> sVars = new();
        List<SyntaxCore> composed = new();
        foreach (var syntax in arguments)
        {
            if (syntax.Type == SyntaxType.Operator && syntax.Name == ",")
            {
                if (composed.Count == 0)
                {
                    throw new CompilationException(CompilationException.ErrorCode.SyntaxError, "Unexpected syntax before ','.", line);
                }

                sVars.Add(SyntaxVariable.CreateFromSyntaxes(composed));
                composed.Clear();
            }
            else
            {
                composed.Add(syntax);
            }
        }

        if (composed.Count != 0)
        {
            sVars.Add(SyntaxVariable.CreateFromSyntaxes(composed));
        }

        return sVars;
    }

    private void ParseClassDeclaration(IEnumerator<SyntaxCore> syntaxes)
    {
        for (; syntaxes.MoveNext();)
        {
            var value = syntaxes.Current;

            if (value.Name == ":")
            {
                ParseClassInheritances(syntaxes);
                break;
            }

            if (value.Name == "{")
            {
                break;
            }

            if (value.Type != SyntaxType.Identifier)
            {
                throw new CompilationException(CompilationException.ErrorCode.SyntaxError, $"Unexpected syntax: {value.Name}", value.Line);
            }

            if (_name != null)
            {
                _apikey = _name;
            }
            _name = value.Name;
        }

        if (_name == null)
        {
            throw new CompilationException(CompilationException.ErrorCode.SyntaxError, "Class name does not specified.", _line);
        }
    }

    private void ParseClassInheritances(IEnumerator<SyntaxCore> syntaxes)
    {
        string? name = null;
        bool isVirtual = false;

        void FlushInh(SyntaxCore value)
        {
            if (name == null)
            {
                throw new CompilationException(CompilationException.ErrorCode.SyntaxError, "Unexpected syntax before ','.", value.Line);
            }

            if (name == "Object")
            {
                if (!isVirtual)
                {
                    throw new CompilationException(CompilationException.ErrorCode.RuleError, "Object must be have virtual modifier.", value.Line);
                }

                _inherit = name;
            }
            else if (isVirtual)
            {
                _interfaces.Add(name);
            }
            else
            {
                if (_inherit != null)
                {
                    throw new CompilationException(CompilationException.ErrorCode.RuleError, "Inheritance the class is allowed only one.", value.Line);
                }

                _inherit = name;
            }

            name = null;
            isVirtual = false;
        }

        SyntaxCore value = default;

        for (; syntaxes.MoveNext();)
        {
            value = syntaxes.Current;

            switch (value.Type)
            {
                case SyntaxType.Operator:
                    if (value.Name == ",")
                    {
                        FlushInh(value);
                    }
                    else
                    {
                        throw new CompilationException(CompilationException.ErrorCode.SyntaxError, $"Unexpected operator {value.Name}", value.Line);
                    }
                    break;
                case SyntaxType.Identifier:
                    if (name != null)
                    {
                        throw new CompilationException(CompilationException.ErrorCode.SyntaxError, "Unexpected identifier before ','.", value.Line);
                    }
                    name = value.Name;
                    break;
                case SyntaxType.Keyword:
                    if (value.Name == "virtual")
                    {
                        isVirtual = true;
                    }
                    else if (value.Name == "protected" || value.Name == "private")
                    {
                        throw new CompilationException(CompilationException.ErrorCode.RuleError, "Class or interface inheritance must be public access.", value.Line);
                    }
                    break;
            }
        }

        if (value.Name != null)
        {
            FlushInh(value);
        }
    }

    private List<string> ParseAttributes(IEnumerator<SyntaxCore> generator)
    {
        var span = generator.EnumerateNext(sCore => sCore.Type == SyntaxType.Operator && sCore.Name == ")", true);
        if (span == null)
        {
            throw new CompilationException(CompilationException.ErrorCode.InvalidSCLASS, "Cannot find parentheses in SCLASS.", _line);
        }

        if (span.Count < 2)
        {
            throw new CompilationException(CompilationException.ErrorCode.InvalidSCLASS, "Parentheses does not opened.", _line);
        }

        return new List<string>();
    }
}
