// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Text;

namespace ReflectionHeaderTool;

internal class CompiledSENUM : CompiledSTYPE
{
    private string? _underlyingType = null;

    public CompiledSENUM(string source)
        : base(source, "SENUM", "enum")
    {
    }

    public override void GenerateHeader(string fileKey, StringBuilder sb)
    {
        // Summary
        sb.AppendLine($"// {_classMacro} for {_name}");
        sb.AppendLine();

        // Supports SCLASS()
        string underlyingType = _underlyingType == null ? "" : (": " + _underlyingType);
        sb.AppendLine($"#define __LIBTY_{_classMacro}__{fileKey}__{_line}__ \\");
        sb.AppendLine($"\\");
        sb.AppendLine($"enum class {_name} {underlyingType};\\");
        sb.AppendLine($"\\");

        if (_typeAttributes.Contains("Flags"))
        {
            sb.AppendLine($"__SENUM_DEFINE_FLAGS_OPERATORS({_name});\\");
        }

        sb.AppendLine();
    }

    public override void GenerateSource(string fileKey, StringBuilder sb)
    {
        // Summary
        sb.AppendLine($"// {_classMacro} for {_name}");
        sb.AppendLine();

        // Does NOT generate anything yet.
    }

    protected override void ParseClassDeclaration(IEnumerator<SyntaxCore> syntaxes)
    {
        syntaxes.MoveNext();
        var value = syntaxes.Current;

        if (value.Type != SyntaxType.Keyword || value.Name != "class")
        {
            throw new CompilationException(CompilationException.ErrorCode.SyntaxError, $"Unexpected syntax: {value.Name}, SENUM() must be \"enum class\".", _source, _line);
        }

        syntaxes.MoveNext();
        value = syntaxes.Current;

        if (value.Type != SyntaxType.Identifier)
        {
            throw new CompilationException(CompilationException.ErrorCode.SyntaxError, $"Unexpected syntax: {value.Name}", _source, _line);
        }

        _name = value.Name;
        _apikey = "";

        syntaxes.MoveNext();
        value = syntaxes.Current;

        if (value.Name == ":")
        {
            syntaxes.MoveNext();
            value = syntaxes.Current;

            _underlyingType = value.Name;
        }
    }

    protected override void ParseClassBody(IEnumerator<SyntaxCore> syntaxes)
    {
        // Need NOT to parsing YET.
    }
}
