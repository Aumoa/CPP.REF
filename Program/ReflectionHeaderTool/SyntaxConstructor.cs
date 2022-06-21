// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace ReflectionHeaderTool;

internal record SyntaxConstructor
{
    public List<SyntaxVariable> Arguments { get; set; } = null!;

    public string SafeName => string
        .Concat(Arguments.Select(p => p.GetComposedName()))
        .Replace(" ", "_1_")
        .Replace("&", "_3_")
        .Replace("*", "_4_");

    public string ComposeInvokeArgs()
    {
        List<string> casts = new();
        for (int i = 0; i < Arguments.Count; ++i)
        {
            casts.Add(MakeCast(Arguments[i], i));
        }
        return string.Join(", ", casts);
    }

    public string ComposeTypeHash()
    {
        if (Arguments.Count == 0)
        {
            return "0";
        }
        else
        {
            return string.Join(" ^ ", Arguments.Select(p => MakeTypeHash(p)));
        }
    }

    private static string MakeCast(SyntaxVariable sv, int index)
    {
        return $"__SCLASS_Cast<{sv.Type}>(args[{index}])";
    }

    private static string MakeTypeHash(SyntaxVariable sv)
    {
        return $"typeid({sv.Type}).hash_code()";
    }
}
