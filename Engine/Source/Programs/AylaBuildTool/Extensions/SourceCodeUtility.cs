// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.IO;
using AE.Source;

namespace AE.Extensions;

public static class SourceCodeUtility
{
    public static bool IsSourceCode(this FileReference file)
    {
        return file.Extensions.ToLower() is "cpp";
    }

    public static bool IsHeaderFile(string name)
    {
        string extensions = Path.GetExtension(name).ToLower();
        return extensions is "h" or "inl";
    }

    public static bool IsRuleFile(string name)
    {
        string extensions = Path.GetExtension(name).ToLower();
        return extensions is "cs";
    }

    public static bool IsNatvisFile(string name)
    {
        string extensions = Path.GetExtension(name).ToLower();
        return extensions is "natvis";
    }

    public static bool IsShaderCode(this FileReference file)
    {
        return file.Extensions.ToLower() is "hlsl";
    }

    public static bool IsShaderHeader(this FileReference file)
    {
        return file.Extensions.ToLower() is "hlsli";
    }

    public static bool IsSourceFile(FileReference name)
    {
        return IsSourceCode(name)
            || IsHeaderFile(name)
            || IsRuleFile(name)
            || IsNatvisFile(name)
            || IsShaderCode(name)
            || IsShaderHeader(name);
    }

    public static bool IsEngineDirectory(this ProjectDirectory projectDir)
    {
        return projectDir.Root == Global.EngineDirectory.Root;
    }
}
