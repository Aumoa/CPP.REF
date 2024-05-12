// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public static class SourceCodeUtility
{
    public static bool IsSourceCode(this FileReference file)
    {
        return file.Extensions.ToLower() is "cpp";
    }

    public static bool IsHeaderFile(this FileReference file)
    {
        return file.Extensions.ToLower() is "h" or "inl";
    }

    public static bool IsRuleFile(this FileReference file)
    {
        return file.Extensions.ToLower() is "module.cs";
    }

    public static bool IsNatvisFile(this FileReference file)
    {
        return file.Extensions.ToLower() is "natvis";
    }

    public static bool IsShaderCode(this FileReference file)
    {
        return file.Extensions.ToLower() is "hlsl";
    }

    public static bool IsShaderHeader(this FileReference file)
    {
        return file.Extensions.ToLower() is "hlsli";
    }

    public static bool IsSourceFile(FileReference file)
    {
        return IsSourceCode(file)
            || IsHeaderFile(file)
            || IsRuleFile(file)
            || IsNatvisFile(file)
            || IsShaderCode(file)
            || IsShaderHeader(file);
    }

    public static bool IsEngineDirectory(this ProjectDirectory projectDir)
    {
        return projectDir.Root == Global.EngineDirectory.Root;
    }
}
