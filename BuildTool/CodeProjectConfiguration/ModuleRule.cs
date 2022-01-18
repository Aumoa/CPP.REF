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
            PublicIncludePaths[i] = Path.Combine(ProjectDir, PublicIncludePaths[i]);
        }

        for (int i = 0; i < PrivateIncludePaths.Count; ++i)
        {
            PrivateIncludePaths[i] = Path.Combine(ProjectDir, PrivateIncludePaths[i]);
        }
    }

    public string GetAPI(bool bExport)
    {
        string ReplacedName = Name.Replace('.', '_');
        string API = ReplacedName.ToUpper() + "_API";
        return API + (bExport ? "=__declspec(dllexport)" : "=__declspec(dllimport)");
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