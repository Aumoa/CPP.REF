// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Text;

namespace ReflectionHeaderTool;

[DebuggerDisplay("SCLASS {_name}({_line})")]
internal class CompiledSCLASS : IHeaderGenerator
{
    private int _line;
    private string _name = null!;
    private string? _inherit;
    private readonly List<string> _interfaces = new();
    private int _generatedBody;

    public CompiledSCLASS()
    {
    }

    public void Compile(IEnumerator<SyntaxCore> generator, List<SyntaxCore> syntaxes)
    {
        _line = syntaxes.Last().Line;

        // Reading SCLASS attributes.
        var span = generator.EnumerateNext(sCore => sCore.Type == SyntaxType.Operator && sCore.Name == ")");
        if (span == null)
        {
            throw new CompilationException(CompilationException.ErrorCode.InvalidSCLASS, "Cannot find parentheses in SCLASS.", _line);
        }

        ParseAttributes(span);

        // Reading class key.
        span = generator.EnumerateNext(sCore => sCore.Type == SyntaxType.Keyword);
        if (span == null)
        {
            throw new CompilationException(CompilationException.ErrorCode.InvalidClassKey, "Cannot find class key for SCLASS.", _line);
        }

        if (span[0].Name != "class")
        {
            throw new CompilationException(CompilationException.ErrorCode.InvalidClassKey, "Class key is not \"class\".", _line);
        }

        span = generator.EnumerateNext(sCore => sCore.Type == SyntaxType.Scope);
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

        sb.AppendLine($"__SCLASS_DECLARE_REFLEXPR({_name})\\");

        sb.AppendLine($"\\");
        sb.AppendLine($"__SCLASS_END_NAMESPACE()");
        sb.AppendLine();

        // Supports GENERATED_BODY()
        sb.AppendLine($"#define __LIBTY_GENERATED_BODY__{fileKey}__{_generatedBody}__ \\");
        sb.AppendLine($"__SCLASS_DECLARE_GENERATED_BODY({_name}, {_inherit ?? "void"})");
        sb.AppendLine();
    }

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

    private void ParseAttributes(List<SyntaxCore> arguments)
    {
        if (arguments.Count < 2)
        {
            throw new CompilationException(CompilationException.ErrorCode.InvalidSCLASS, "Parentheses does not opened.", _line);
        }
    }
}
