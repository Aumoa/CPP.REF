﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace ReflectionHeaderTool;

internal record SyntaxVariable
{
    public string Type { get; set; } = null!;

    public string Name { get; set; } = null!;

    public string? DefaultValue { get; set; }

    public string GetComposedName()
    {
        return Type + (Name is null ? "" : ' ' + Name);
    }

    public static SyntaxVariable CreateFromSyntaxes(List<SyntaxCore> syntaxes, string source)
    {
        SyntaxVariable sVar = new();

        string composed = "";
        bool typeHit = false;
        int templateLevel = 0;
        bool inNamespace = false;
        SyntaxType? previous = null;

        foreach (var syntax in syntaxes)
        {
            if (sVar.Name == null)
            {
                if (syntax.Type == SyntaxType.Identifier && inNamespace == false && templateLevel == 0)
                {
                    if (typeHit == false)
                    {
                        typeHit = true;
                    }
                    else
                    {
                        sVar.Type = composed.Trim();
                        composed = "";
                    }
                }
                else if (syntax.Type == SyntaxType.Operator)
                {
                    if (syntax.Name == "=")
                    {
                        if (sVar.Type == null)
                        {
                            throw new CompilationException(CompilationException.ErrorCode.SyntaxError, "Variable identifier does not exists before '='.", source, syntax.Line);
                        }

                        sVar.Name = composed.Trim();
                        composed = "";
                        continue;
                    }
                    else if (syntax.Name == ":")
                    {
                        inNamespace = true;
                    }
                    else if (syntax.Name == ">")
                    {
                        templateLevel++;
                    }
                    else if (syntax.Name == "<")
                    {
                        templateLevel--;
                    }
                }
                else
                {
                    inNamespace = false;
                }
            }

            if (previous == syntax.Type && syntax.Type != SyntaxType.Operator)
            {
                composed += ' ';
            }
            composed += syntax.Name;
            previous = syntax.Type;
        }

        if (sVar.Type == null)
        {
            sVar.Type = composed.Trim();
        }
        else if (sVar.Name == null)
        {
            sVar.Name = composed.Trim();
        }
        else
        {
            sVar.DefaultValue = composed.Trim();
        }

        return sVar;
    }
}
