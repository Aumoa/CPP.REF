// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Xml;

/// <summary>
/// 엔진 프로젝트 솔루션을 생성하는 하위 프로그램입니다.
/// </summary>
class GenerateEngineSolution : Subprogram
{
    static readonly string SolutionFilterGuid = "2150E333-8FDC-42A3-9474-1A3956D46DE8";
    static readonly string CppProjectGuid = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942";
    static readonly string CsProjectGuid = "9A19103F-16F7-4668-BE54-9A1E7A4F7556";

    DirectoryReference RootDirectory;
    DirectoryReference EngineDirectory;
    DirectoryReference RuntimeDirectory;
    DirectoryReference ProgramDirectory;
    DirectoryReference ThirdPartyDirectory;

    StringBuilder Builder = new();

    /// <summary>
    /// 개체를 생성합니다.
    /// </summary>
    /// <param name="InArgs"> 명령 인수를 전달합니다. </param>
    public GenerateEngineSolution(string[] InArgs)
    {
        if (InArgs.Length < 2)
        {
            throw new ArgumentException("GenerateEngineSolution 두 번째 인수는 프로젝트의 루트 디렉토리입니다.");
        }

        RootDirectory = new(InArgs[1]);
        if (!RootDirectory.IsExist)
        {
            throw new DirectoryNotExistsException(RootDirectory.FullPath);
        }

        EngineDirectory = RootDirectory.Move("Engine");
        if (!EngineDirectory.IsExist)
        {
            throw new DirectoryNotExistsException(EngineDirectory.FullPath);
        }

        RuntimeDirectory = EngineDirectory.Move("Source/Runtime");
        if (!RuntimeDirectory.IsExist)
        {
            throw new DirectoryNotExistsException(RuntimeDirectory.FullPath);
        }

        ProgramDirectory = EngineDirectory.Move("Source/Program");
        if (!ProgramDirectory.IsExist)
        {
            throw new DirectoryNotExistsException(ProgramDirectory.FullPath);
        }

        ThirdPartyDirectory = RootDirectory.Move("ThirdParty");
        if (!ThirdPartyDirectory.IsExist)
        {
            throw new DirectoryNotExistsException(ThirdPartyDirectory.FullPath);
        }
    }

    readonly Guid EngineGuid = Guid.NewGuid();
    readonly Guid ProgramGuid = Guid.NewGuid();
    readonly Guid ThirdPartyGuid = Guid.NewGuid();

    /// <summary>
    /// 프로그램을 실행합니다.
    /// </summary>
    public override void Run()
    {
        GenerateThirdPartyProjects();
        GenerateProgramProjects();
        GenerateRuntimeProjects();

        // 솔루션 헤더를 씁니다.
        Builder.AppendLine("Microsoft Visual Studio Solution File, Format Version 12.00");
        Builder.AppendLine("# Visual Studio Version 16");
        Builder.AppendLine("VisualStudioVersion = 16.0.0");
        Builder.AppendLine("MinimumVisualStudioVersion = 16.0.0");

        // 솔루션 필터를 우선 생성합니다.
        WriteSolutionFilter("Engine", EngineGuid);
        WriteSolutionFilter("Program", ProgramGuid);
        WriteSolutionFilter("ThirdParty", ThirdPartyGuid);
        foreach (var Item in RuntimeProjects)
        {
            WriteProjectFile(Item.Value);
        }
        foreach (var Item in ProgramProjects)
        {
            WriteProjectFile(Item.Value);
        }
        foreach (var Item in ThirdPartyProjects)
        {
            WriteProjectFile(Item.Value);
        }

        Builder.AppendLine($"Global");
        WriteSolutionConfigurationPlatforms();
        WriteProjectConfigurationPlatforms();
        WriteNestedProjects();
        Builder.AppendLine($"EndGlobal");

        // 솔루션 파일을 씁니다.
        string OutputPath = Path.Combine(RootDirectory.FullPath, "Engine.sln");
        File.WriteAllText(OutputPath, Builder.ToString());
        Console.WriteLine(OutputPath);
    }

    /// <summary>
    /// 솔루션 설정 및 플랫폼을 생성합니다.
    /// </summary>
    private void WriteSolutionConfigurationPlatforms()
    {
        Builder.AppendLine($"\tGlobalSection(SolutionConfigurationPlatforms) = preSolution");
        Builder.AppendLine($"\t\tDebug|x64 = Debug|x64");
        Builder.AppendLine($"\t\tDebugGame|x64 = DebugGame|x64");
        Builder.AppendLine($"\t\tRelease|x64 = Release|x64");
        Builder.AppendLine($"\tEndGlobalSection");
    }

    /// <summary>
    /// 프로젝트 설정 및 플랫폼을 생성합니다.
    /// </summary>
    private void WriteProjectConfigurationPlatforms()
    {
        Builder.AppendLine($"\tGlobalSection(SolutionConfigurationPlatforms) = postSolution");
        foreach (var Item in RuntimeProjects)
        {
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Debug|x64.ActiveCfg = Debug|x64");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Debug|x64.Build.0 = Debug|x64");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.DebugGame|x64.ActiveCfg = Release|x64");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.DebugGame|x64.Build.0 = Release|x64");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Release|x64.ActiveCfg = Release|x64");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Release|x64.Build.0 = Release|x64");
        }
        foreach (var Item in ProgramProjects)
        {
            bool bCppProject = Item.Value is CppProjectBuildInfo;
            string PlatformStr = bCppProject ? "x64" : "Any CPU";
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Debug|x64.ActiveCfg = Debug|{PlatformStr}");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Debug|x64.Build.0 = Debug|{PlatformStr}");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.DebugGame|x64.ActiveCfg = Release|{PlatformStr}");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.DebugGame|x64.Build.0 = Release|{PlatformStr}");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Release|x64.ActiveCfg = Release|{PlatformStr}");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Release|x64.Build.0 = Release|{PlatformStr}");
        }
        foreach (var Item in ThirdPartyProjects)
        {
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Debug|x64.ActiveCfg = Debug|x64");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Debug|x64.Build.0 = Debug|x64");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.DebugGame|x64.ActiveCfg = Release|x64");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.DebugGame|x64.Build.0 = Release|x64");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Release|x64.ActiveCfg = Release|x64");
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid}}}.Release|x64.Build.0 = Release|x64");
        }
        Builder.AppendLine($"\tEndGlobalSection");
    }

    /// <summary>
    /// 프로젝트 포함 관계를 생성합니다.
    /// </summary>
    private void WriteNestedProjects()
    {
        Builder.AppendLine($"\tGlobalSection(NestedProjects) = preSolution");
        foreach (var Item in RuntimeProjects)
        {
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid.ToString().ToUpper()}}} = {{{EngineGuid.ToString().ToUpper()}}}");
        }
        foreach (var Item in ProgramProjects)
        {
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid.ToString().ToUpper()}}} = {{{ProgramGuid.ToString().ToUpper()}}}");
        }
        foreach (var Item in ThirdPartyProjects)
        {
            Builder.AppendLine($"\t\t{{{Item.Value.ProjectGuid.ToString().ToUpper()}}} = {{{ThirdPartyGuid.ToString().ToUpper()}}}");
        }
        Builder.AppendLine($"\tEndGlobalSection");
    }

    /// <summary>
    /// 프로젝트를 생성합니다.
    /// </summary>
    /// <param name="Target"> 프로젝트를 전달합니다.</param>
    private void WriteProjectFile(ProjectBuildInfo Target)
    {
        bool bCppProject = Target is CppProjectBuildInfo;
        string TypeGuid = bCppProject ? CppProjectGuid : CsProjectGuid;

        Builder.AppendLine($"Project(\"{{{TypeGuid}}}\") = \"{Target.Symbol}\", \"{Target.ProjectFile.FullPath}\", \"{{{Target.ProjectGuid.ToString().ToUpper()}}}\"");
        Builder.AppendLine($"EndProject");
    }

    /// <summary>
    /// 솔루션 필터를 생성합니다.
    /// </summary>
    /// <param name="Name"> 필터 이름을 전달합니다. </param>
    /// <param name="FilterGuid"> 필터 GUID를 전달합니다. </param>
    private void WriteSolutionFilter(string Name, Guid FilterGuid)
    {
        Builder.AppendLine($"Project(\"{{{SolutionFilterGuid}}}\") = \"{Name}\", \"{Name}\", \"{{{FilterGuid}}}\"");
        Builder.AppendLine($"EndProject");
    }

    Dictionary<string, CppProjectBuildInfo> AllProjects = new();
    Dictionary<string, CppProjectBuildInfo> ThirdPartyProjects = new();

    /// <summary>
    /// ThirdParty 프로젝트를 수집합니다.
    /// </summary>
    private void GenerateThirdPartyProjects()
    {
        Log.TraceLog($"{ThirdPartyDirectory.FullPath} 경로의 소스코드 프로젝트 탐색을 시작합니다.");

        FileReference[] Files = ThirdPartyDirectory.GetAllFiles();
        if (Files is null || Files.Length == 0)
        {
            throw new FileNotFoundException("필요한 파일이 존재하지 않습니다.");
        }

        FileReference BuileRuleAssembly = new(typeof(BuildRules).Assembly.Location);

        // 프로젝트 레퍼런스를 수집합니다.
        foreach (FileReference File in Files)
        {
            if (IsProjectBuildReference(File, out var Symbol))
            {
                Assembly BuildRulesAssembly = RuntimeCompilation.CompileAssembly(
                    File.Name,
                    new List<FileReference>() { File },
                    new List<FileReference>() { BuileRuleAssembly }
                    );

                CppProjectBuildInfo BuildInfo = new();
                BuildInfo.SourceRoot = File.GetParent();
                BuildInfo.Rules = Activator.CreateInstance(BuildRulesAssembly.GetType($"{Symbol}Build")) as BuildRules;
                BuildInfo.Symbol = Symbol;

                ThirdPartyProjects.Add(Symbol, BuildInfo);
                AllProjects.Add(Symbol, BuildInfo);
            }
        }

        // 프로젝트의 의존 관계를 해결합니다.
        foreach (var Item in ThirdPartyProjects)
        {
            CppProjectBuildInfo BuildInfo = Item.Value;
            ResolveProjectDependency(BuildInfo);
        }

        // 프로젝트 파일을 생성합니다.
        foreach (var Item in ThirdPartyProjects)
        {
            CppProjectBuildInfo BuildInfo = Item.Value;
            GenerateCppProject(BuildInfo, ThirdPartyDirectory);
        }
    }

    Dictionary<string, ProjectBuildInfo> ProgramProjects = new();

    /// <summary>
    /// 프로그램 프로젝트를 수집합니다.
    /// </summary>
    private void GenerateProgramProjects()
    {
        Log.TraceLog($"{ProgramDirectory.FullPath} 경로의 소스코드 프로젝트 탐색을 시작합니다.");

        FileReference[] Files = ProgramDirectory.GetAllFiles();
        if (Files is null || Files.Length == 0)
        {
            throw new FileNotFoundException("필요한 파일이 존재하지 않습니다.");
        }

        FileReference BuileRuleAssembly = new(typeof(BuildRules).Assembly.Location);

        // 프로젝트 레퍼런스를 수집합니다.
        foreach (FileReference File in Files)
        {
            // C# 프로젝트입니다.
            if (File.CompareExtension("csproj"))
            {
                CsProjectBuildInfo BuildInfo = new();
                BuildInfo.SourceRoot = File.GetParent();
                BuildInfo.Symbol = File.Name;
                BuildInfo.ProjectFile = File;
                BuildInfo.ProjectGuid = Guid.NewGuid();

                ProgramProjects.Add(BuildInfo.Symbol, BuildInfo);
            }

            // C++ 프로젝트 빌드 cs 파일입니다.
            if (IsProjectBuildReference(File, out var Symbol))
            {
                Assembly BuildRulesAssembly = RuntimeCompilation.CompileAssembly(
                    File.Name,
                    new List<FileReference>() { File },
                    new List<FileReference>() { BuileRuleAssembly }
                    );

                CppProjectBuildInfo BuildInfo = new();
                BuildInfo.SourceRoot = File.GetParent();
                BuildInfo.Rules = Activator.CreateInstance(BuildRulesAssembly.GetType($"{Symbol}Build")) as BuildRules;
                BuildInfo.Symbol = Symbol;

                ProgramProjects.Add(Symbol, BuildInfo);
                AllProjects.Add(Symbol, BuildInfo);
            }
        }

        // 프로젝트의 의존 관계를 해결합니다.
        foreach (var Item in ProgramProjects)
        {
            if (Item.Value is CppProjectBuildInfo BuildInfo)
            {
                ResolveProjectDependency(BuildInfo);
            }
        }

        // 프로젝트 파일을 생성합니다.
        foreach (var Item in ProgramProjects)
        {
            if (Item.Value is CppProjectBuildInfo BuildInfo)
            {
                GenerateCppProject(BuildInfo, EngineDirectory);
            }
        }
    }

    Dictionary<string, CppProjectBuildInfo> RuntimeProjects = new();

    /// <summary>
    /// Engine C++ 프로젝트를 수집합니다.
    /// </summary>
    private void GenerateRuntimeProjects()
    {
        Log.TraceLog($"{RuntimeDirectory.FullPath} 경로의 소스코드 프로젝트 탐색을 시작합니다.");

        FileReference[] Files = RuntimeDirectory.GetAllFiles();
        if (Files is null || Files.Length == 0)
        {
            throw new FileNotFoundException("필요한 파일이 존재하지 않습니다.");
        }

        FileReference BuileRuleAssembly = new(typeof(BuildRules).Assembly.Location);

        // 프로젝트 레퍼런스를 수집합니다.
        foreach (FileReference File in Files)
        {
            if (IsProjectBuildReference(File, out var Symbol))
            {
                Assembly BuildRulesAssembly = RuntimeCompilation.CompileAssembly(
                    File.Name,
                    new List<FileReference>() { File },
                    new List<FileReference>() { BuileRuleAssembly }
                    );

                CppProjectBuildInfo BuildInfo = new();
                BuildInfo.SourceRoot = File.GetParent();
                BuildInfo.Rules = Activator.CreateInstance(BuildRulesAssembly.GetType($"{Symbol}Build")) as BuildRules;
                BuildInfo.Symbol = Symbol;

                RuntimeProjects.Add(Symbol, BuildInfo);
                AllProjects.Add(Symbol, BuildInfo);
            }
        }

        // 프로젝트의 의존 관계를 해결합니다.
        foreach (var Item in RuntimeProjects)
        {
            CppProjectBuildInfo BuildInfo = Item.Value;
            ResolveProjectDependency(BuildInfo);
        }

        // 프로젝트 파일을 생성합니다.
        foreach (var Item in RuntimeProjects)
        {
            CppProjectBuildInfo BuildInfo = Item.Value;
            GenerateCppProject(BuildInfo, EngineDirectory);
        }
    }

    /// <summary>
    /// 단일 프로젝트를 생성합니다.
    /// </summary>
    /// <param name="Target"> 대상 프로젝트를 전달합니다. </param>
    private void GenerateCppProject(CppProjectBuildInfo Target, DirectoryReference SourceRoot)
    {
        // 이 프로젝트는 이미 생성되었습니다.
        if (Target.IsGenerated)
        {
            return;
        }

        DirectoryReference IntermediateRoot = RootDirectory.Move(@"Intermediate\ProjectFiles");

        // 의존 관계 프로젝트를 포함합니다.
        List<CppProjectBuildInfo> AllDependency = new();
        foreach (string Module in Target.ResolvedPrivateDependency)
        {
            AllDependency.Add(AllProjects[Module]);
        }
        foreach (string Module in Target.ResolvedPublicDependency)
        {
            AllDependency.Add(AllProjects[Module]);
        }

        // 생성되지 않은 프로젝트가 있다면 생성합니다.
        foreach (CppProjectBuildInfo Build in AllDependency)
        {
            GenerateCppProject(Build, SourceRoot);
        }

        CppProjectGenerator Generator = new(Target.Symbol, SourceRoot, Target.SourceRoot, IntermediateRoot);
        Target.ProjectFile = Generator.Generate(Target, AllDependency);
        Target.ProjectGuid = GetCppProjectGuid(Target.ProjectFile.FullPath);
    }

    /// <summary>
    /// 단일 프로젝트의 의존 관계를 해결합니다.
    /// </summary>
    /// <param name="Target"> 대상 프로젝트를 전달합니다. </param>
    private void ResolveProjectDependency(CppProjectBuildInfo Target)
    {
        // 이미 처리된 프로젝트입니다.
        if (Target.ResolvedPublicDependency is not null)
        {
            return;
        }

        Target.ResolvedPrivateDependency = new();
        Target.ResolvedPublicDependency = new();

        // 해당 프로젝트의 공개 의존 관계 목록을 구성합니다.
        for (int i = 0; i < Target.Rules.PublicDependencyModuleNames.Count; ++i)
        {
            string PublicModuleSymbol = Target.Rules.PublicDependencyModuleNames[i];
            CppProjectBuildInfo PublicModule = AllProjects[PublicModuleSymbol];
            
            // 의존 관계 대상 프로젝트가 처리되지 않았으면 먼저 처리합니다.
            if (PublicModule.ResolvedPublicDependency is null)
            {
                ResolveProjectDependency(PublicModule);
            }

            Target.ResolvedPublicDependency.AddRange(PublicModule.ResolvedPublicDependency);
            Target.ResolvedPublicDependency.Add(PublicModuleSymbol);
        }

        // 해당 프로젝트의 비공개 의존 관계 목록을 구성합니다.
        for (int i = 0; i < Target.Rules.PrivateDependencyModuleNames.Count; ++i)
        {
            string PrivateModuleSymbol = Target.Rules.PrivateDependencyModuleNames[i];
            CppProjectBuildInfo PrivateModule = AllProjects[PrivateModuleSymbol];

            // 의존 관계 대상 프로젝트가 처리되지 않았으면 먼저 처리합니다.
            if (PrivateModule.ResolvedPublicDependency is null)
            {
                ResolveProjectDependency(PrivateModule);
            }

            // 의존 프로젝트의 공개 의존 목록을 비공개 의존 목록에 포함합니다. 이는 오류가 아닙니다.
            // 비공개 의존 목록의 공개 의존 목록은 이후 이 프로젝트를 참조하는 프로젝트에게 공개될 필요가 없습니다.
            Target.ResolvedPrivateDependency.AddRange(PrivateModule.ResolvedPublicDependency);
            Target.ResolvedPrivateDependency.Add(PrivateModuleSymbol);
        }

        // 중복 모듈을 제거합니다.
        Target.ResolvedPrivateDependency = Target.ResolvedPrivateDependency.Distinct().ToList();
        Target.ResolvedPublicDependency = Target.ResolvedPublicDependency.Distinct().ToList();

        for (int i = 0; i < Target.ResolvedPrivateDependency.Count; ++i)
        {
            string Private = Target.ResolvedPrivateDependency[i];
            if (Target.ResolvedPublicDependency.Contains(Private))
            {
                Target.ResolvedPrivateDependency.RemoveAt(i);
                i -= 1;
            }
        }
    }

    /// <summary>
    /// 파일이 프로젝트 빌드 레퍼런스(Build.cs)인지 검사합니다.
    /// </summary>
    /// <param name="InFile"> 파일 레퍼런스를 전달합니다. </param>
    /// <returns> 결과가 반환됩니다. </returns>
    private bool IsProjectBuildReference(FileReference InFile, out string Symbol)
    {
        Symbol = null;

        // 디렉토리에 .Build.cs 파일이 포함됩니다.
        if (!InFile.FileName.EndsWith(".Build.cs"))
        {
            return false;
        }

        // 상위 디렉토리의 이름이 파일 이름과 일치해야 합니다.
        DirectoryReference Parent = InFile.GetParent();
        if (Parent is null || !$"{Parent.Name}.Build".Equals(InFile.Name, StringComparison.OrdinalIgnoreCase))
        {
            return false;
        }

        Symbol = InFile.Name.Remove(InFile.Name.IndexOf(".Build"));
        return true;
    }

    /// <summary>
    /// CPP 프로젝트의 GUID를 파싱합니다.
    /// </summary>
    /// <param name="ProjectPath"> 프로젝트 경로를 전달합니다. </param>
    /// <returns> Guid가 반환됩니다. </returns>
    private Guid GetCppProjectGuid(string ProjectPath)
    {
        XmlDocument ProjectDoc = new();
        ProjectDoc.Load(ProjectPath);
        string ProjectGuid = null;
        foreach (XmlNode Node in ProjectDoc.FirstChild.ChildNodes)
        {
            if (Node.Name == "PropertyGroup")
            {
                foreach (XmlNode Child in Node.ChildNodes)
                {
                    if (Child.Name == "ProjectGuid")
                    {
                        ProjectGuid = Child.InnerText;
                        break;
                    }
                }

                if (!string.IsNullOrEmpty(ProjectGuid))
                {
                    break;
                }
            }
        }

        return Guid.Parse(ProjectGuid.TrimStart('{').TrimEnd('}'));
    }
}