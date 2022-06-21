// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace ReflectionHeaderTool;

internal record SyntaxVariable
{
    public string Type { get; set; } = null!;

    public string Name { get; set; } = null!;

    public string GetComposedName()
    {
        return Type + (Name is null ? "" : ' ' + Name);
    }

    public static SyntaxVariable CreateFromSyntaxes(List<SyntaxCore> syntaxes)
    {
        SyntaxVariable sVar = new();

        string composed = "";
        bool typeHit = false;
        foreach (var syntax in syntaxes)
        {
            if (syntax.Type == SyntaxType.Identifier)
            {
                if (typeHit == false)
                {
                    typeHit = true;
                }
                else
                {
                    sVar.Type = composed;
                    composed = "";
                }
            }

            composed += ' ' + syntax.Name;
        }

        if (sVar.Type == null)
        {
            sVar.Type = composed;
        }
        else
        {
            sVar.Name = composed;
        }

        return sVar;
    }
}
