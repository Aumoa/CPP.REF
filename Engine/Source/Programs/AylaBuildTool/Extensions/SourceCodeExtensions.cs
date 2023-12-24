// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.Source;

namespace AE.Extensions;

public static class SourceCodeExtensions
{
    public static bool IsSourceCode(string Name)
    {
        string Ext = Path.GetExtension(Name).ToLower();
        return Ext is ".cpp";
    }

    public static bool IsHeaderFile(string Name)
    {
        string Ext = Path.GetExtension(Name).ToLower();
        return Ext is ".h" or ".inl";
    }

    public static bool IsRuleFile(string Name)
    {
        string Ext = Path.GetExtension(Name).ToLower();
        return Ext is ".cs";
    }

    public static bool IsNatvisFile(string Name)
    {
        string Ext = Path.GetExtension(Name).ToLower();
        return Ext is ".natvis";
    }

    public static bool IsShaderFile(string Name)
    {
        string Ext = Path.GetExtension(Name).ToLower();
        return Ext is ".hlsl" or ".hlsli";
    }

    public static bool IsSourceFile(string Name)
    {
        return IsSourceCode(Name)
            || IsHeaderFile(Name)
            || IsRuleFile(Name)
            || IsNatvisFile(Name);
    }

    public static bool IsEngineDirectory(this ProjectDirectory ProjectDir)
    {
        return ProjectDir.Root == Global.EngineDirectory.Root;
    }
}
