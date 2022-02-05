// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace CodeProjectConfiguration;

public class ModuleRule
{
    public ModuleRule()
    {
        Name = GetType().Name;
        TargetName = GetType().Name;
        RelativePath = "Game";
        TargetType = TargetType.Game;
        ModuleType = ModuleType.GameModule;
    }

    public void GenerateAdditionalIncludeDirectories(string ProjectDir)
    {
        for (int i = 0; i < PublicIncludePaths.Count; ++i)
        {
            PublicIncludePaths[i] = FromUniversalPath(ProjectDir, PublicIncludePaths[i]);
        }

        for (int i = 0; i < PrivateIncludePaths.Count; ++i)
        {
            PrivateIncludePaths[i] = FromUniversalPath(ProjectDir, PrivateIncludePaths[i]);
        }
    }

    public void GenerateAdditionalLibraries(string ProjectDir)
    {
        for (int i = 0; i < PublicAdditionalLibraries.Count; ++i)
        {
            PublicAdditionalLibraries[i] = ReplaceEnvironmentPath(PublicAdditionalLibraries[i]);
        }

        for (int i = 0; i < PrivateAdditionalLibraries.Count; ++i)
        {
            PrivateAdditionalLibraries[i] = ReplaceEnvironmentPath(PrivateAdditionalLibraries[i]);
        }
    }

    public string GetAPI(bool bExport)
    {
        string ReplacedName = Name.Replace('.', '_');
        string API = ReplacedName.ToUpper() + "_API";
        return API + (bExport ? "=__declspec(dllexport)" : "=__declspec(dllimport)");
    }

    static private string ReplaceEnvironmentPath(string InPath)
    {
        for (; ; )
        {
            int IndexOfStart = InPath.IndexOf('%');
            if (IndexOfStart == -1)
            {
                break;
            }

            int IndexOfEnd = InPath.IndexOf('%', IndexOfStart + 1);
            if (IndexOfEnd == -1)
            {
                throw new ArgumentException("Environment variable specifier(%) is not match with close scope.");
            }
            string EnvName = InPath.Substring(IndexOfStart + 1, IndexOfEnd - IndexOfStart - 1);
            string EnvPath = Environment.GetEnvironmentVariable(EnvName);

            InPath = InPath.Replace($"%{EnvName}%", EnvPath);
        }

        return InPath;
    }

    static private string FromUniversalPath(string ProjectDir, string InPath)
    {
        InPath = ReplaceEnvironmentPath(InPath);

        if (InPath.StartsWith("@"))
        {
            return InPath[1..];
        }
        else
        {
            return Path.Combine(ProjectDir, InPath);
        }
    }

    public string Name { get; set; }
    public string TargetName { get; set; }
    public string RelativePath { get; set; }
    public TargetType TargetType { get; set; }
    public ModuleType ModuleType { get; set; }

    public List<string> PublicDependencyModuleNames { get; set; } = new();
    public List<string> PrivateDependencyModuleNames { get; set; } = new();

    public List<string> PublicIncludePaths { get; set; } = new();
    public List<string> PrivateIncludePaths { get; set; } = new();

    public List<int> PublicDisableWarnings { get; set; } = new();
    public List<int> PrivateDisableWarnings { get; set; } = new();

    public List<string> PublicPreprocessorDefines { get; set; } = new();
    public List<string> PrivatePreprocessorDefines { get; set; } = new();

    public List<string> PublicAdditionalLibraries { get; set; } = new();
    public List<string> PrivateAdditionalLibraries { get; set; } = new();
}