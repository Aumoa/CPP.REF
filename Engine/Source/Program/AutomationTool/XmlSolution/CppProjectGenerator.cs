// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

/// <summary>
/// 단일 CPP 프로젝트 파일을 생성합니다.
/// </summary>
class CppProjectGenerator
{
    public enum CppProjectGeneratorVersion
    {
        VC2017,
        VC2019
    }

    readonly static Dictionary<CppProjectGeneratorVersion, string> VCProjectVersion = new()
    {
        [CppProjectGeneratorVersion.VC2019] = "16.0"
    };

    readonly static Dictionary<CppProjectGeneratorVersion, string> PlatformToolset = new()
    {
        [CppProjectGeneratorVersion.VC2019] = "v142"
    };

    string ProjectName;
    DirectoryReference RootDirectory;
    DirectoryReference SourceDirectory;
    DirectoryReference OutputRoot;

    XmlDocument XmlVcxProj;
    XmlDocument XmlVcxProjFilter;

    List<FileReference> SourceCodeFiles = new();
    List<FileReference> HeaderFiles = new();
    List<FileReference> FxCodeFiles = new();
    string ProjectFilter;

    FileReference ProjectReference;

    public CppProjectGenerator(string InProjectName, DirectoryReference InRootDirectory, DirectoryReference InSourceDirectory, DirectoryReference InOutputRoot)
    {
        ProjectName = InProjectName;
        RootDirectory = InRootDirectory;
        SourceDirectory = InSourceDirectory;
        OutputRoot = InOutputRoot;

        ProjectFilter = InRootDirectory.Move("Source").GetRelativePath(SourceDirectory);
    }

    /// <summary>
    /// 프로젝트 파일 및 프로젝트 필터 파일을 생성합니다.
    /// </summary>
    /// <param name="GeneratedDependencyProjects"> 의존 관계 프로젝트 목록을 전달합니다. </param>
    public FileReference Generate(CppProjectBuildInfo MyBuild, List<CppProjectBuildInfo> GeneratedDependencyProjects)
    {
        FileReference[] AllFiles = SourceDirectory.GetAllFiles();
        foreach (FileReference File in AllFiles)
        {
            if (MyBuild.Rules.ExtensionNamesForCompile.Contains(File.Extension))
            {
                SourceCodeFiles.Add(File);
            }
            else if (MyBuild.Rules.ExtensionNamesForFxCompile.Contains(File.Extension))
            {
                FxCodeFiles.Add(File);
            }
            else
            {
                HeaderFiles.Add(File);
            }
        }

        GenerateVcxProj(MyBuild, GeneratedDependencyProjects);
        GenerateVcxProjFilter();

        return ProjectReference;
    }

    /// <summary>
    /// 프로젝트 파일을 생성합니다.
    /// </summary>
    /// <param name="MyBuild"> 이 프로젝트의 빌드 정보를 전달합니다. </param>
    /// <param name="GeneratedDependencyProjects"> 의존 관계 프로젝트 목록을 전달합니다. </param>
    private void GenerateVcxProj(CppProjectBuildInfo MyBuild, List<CppProjectBuildInfo> GeneratedDependencyProjects)
    {
        XmlVcxProj = new XmlDocument();
        XmlDocument MyDoc = XmlVcxProj;

        XmlNode RootNode = GenerateProjectNode(MyDoc);
        GenerateItemGroupNode(RootNode);
        GeneratePropertyGroupsGlobal(RootNode);
        GenerateImportProject(RootNode, "$(VCTargetsPath)\\Microsoft.Cpp.Default.props");
        GeneratePropertySheets(RootNode, MyBuild.Rules.bLaunch);
        GenerateImportProject(RootNode, "$(VCTargetsPath)\\Microsoft.Cpp.props");
        GeneratePropertyGroup(RootNode, false, GeneratedDependencyProjects);
        GeneratePropertyGroup(RootNode, true, GeneratedDependencyProjects);
        XmlNode PropertySheets = GenerateImportGroup(RootNode, "PropertySheets");
        GenerateImportProject(PropertySheets, "$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props", "LocalAppDataPlatform");
        GenerateItemDefinitionGroup(RootNode, MyBuild, GeneratedDependencyProjects);
        GenerateItemDefinitionGroup(RootNode, true, MyBuild, GeneratedDependencyProjects);
        GenerateItemDefinitionGroup(RootNode, false, MyBuild, GeneratedDependencyProjects);
        GenerateHeaderFiles(RootNode);
        GenerateFxCodeFiles(RootNode, MyBuild);
        GenerateSourceFiles(RootNode, MyBuild);
        GenerateImportProject(RootNode, "$(VCTargetsPath)\\Microsoft.Cpp.targets");
        GenerateProjectReferences(RootNode, GeneratedDependencyProjects);

        ProjectReference = new FileReference(SaveAll(MyDoc, $"{ProjectName}.vcxproj"));
    }

    /// <summary>
    /// 프로젝트 필터 파일을 생성합니다.
    /// </summary>
    private void GenerateVcxProjFilter()
    {
        XmlVcxProjFilter = new XmlDocument();
        XmlDocument MyDoc = XmlVcxProjFilter;

        XmlNode RootNode = GenerateProjectNode(MyDoc, true);
        GenerateFilters(RootNode);
        GenerateIncludeFiles(RootNode);
        GenerateFxCodeFiles(RootNode);
        GenerateCodeFiles(RootNode);

        SaveAll(MyDoc, $"{ProjectName}.vcxproj.filters");
    }

    /// <summary>
    /// Xml 문서를 저장합니다.
    /// </summary>
    /// <param name="InDoc"> Xml 문서를 전달합니다. </param>
    /// <param name="InFilename"> 파일 이름을 전달합니다. </param>
    private string SaveAll(XmlDocument InDoc, string InFilename)
    {
        string OutputDirectory = OutputRoot.FullPath;
        if (!string.IsNullOrEmpty(ProjectFilter))
        {
            OutputDirectory = Path.Combine(OutputDirectory, ProjectFilter);
        }

        string OutputPath = Path.Combine(OutputDirectory, InFilename);

        // 디렉토리가 존재하지 않을 경우 새로 만듭니다.
        if (!Directory.Exists(OutputDirectory))
        {
            Directory.CreateDirectory(OutputDirectory);
        }

        InDoc.Save(OutputPath);
        Console.WriteLine(OutputPath);

        return OutputPath;
    }

    /// <summary>
    /// 프로젝트의 의존 레퍼런스를 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <param name="GeneratedDependencyProjects"> 의존 관계 프로젝트 목록을 전달합니다. </param>
    private void GenerateProjectReferences(XmlNode InParent, List<CppProjectBuildInfo> GeneratedDependencyProjects)
    {
        if (GeneratedDependencyProjects.Count == 0)
        {
            return;
        }

        XmlNode Group = CreateChildNode(InParent, "ItemGroup");
        foreach (CppProjectBuildInfo Dependency in GeneratedDependencyProjects)
        {
            if (!Dependency.Rules.bDirectLink)
            {
                XmlNode ProjectReference = CreateChildNode(Group, "ProjectReference");
                CreateKeyStringAttribute(ProjectReference, "Include", Dependency.ProjectFile.FullPath);
                CreateChildNode(ProjectReference, "Project").InnerText = $"{{{Dependency.ProjectGuid}}}";
            }
        }
    }

    /// <summary>
    /// 소스 코드 목록에 포함된 헤더 파일을 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    private void GenerateIncludeFiles(XmlNode InParent)
    {
        XmlNode Group = CreateChildNode(InParent, "ItemGroup");

        for (int i = 0; i < HeaderFiles.Count; ++i)
        {
            string CodeRelativePath = SourceDirectory.GetRelativePath(HeaderFiles[i]);
            string RelativePath = Path.GetDirectoryName(CodeRelativePath);

            XmlNode ClInclude = CreateChildNode(Group, "ClInclude");
            CreateKeyStringAttribute(ClInclude, "Include", HeaderFiles[i].FullPath);
            CreateChildNode(ClInclude, "Filter").InnerText = RelativePath;
        }
    }

    /// <summary>
    /// FX 코드 목록에 포함된 코드 파일을 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    private void GenerateFxCodeFiles(XmlNode InParent)
    {
        XmlNode Group = CreateChildNode(InParent, "ItemGroup");

        for (int i = 0; i < FxCodeFiles.Count; ++i)
        {
            string CodeRelativePath = SourceDirectory.GetRelativePath(FxCodeFiles[i]);
            string RelativePath = Path.GetDirectoryName(CodeRelativePath);

            XmlNode FxCompile = CreateChildNode(Group, "FxCompile");
            CreateKeyStringAttribute(FxCompile, "Include", FxCodeFiles[i].FullPath);
            CreateChildNode(FxCompile, "Filter").InnerText = RelativePath;
        }
    }

    /// <summary>
    /// 소스 코드 목록에 포함된 코드 파일을 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    private void GenerateCodeFiles(XmlNode InParent)
    {
        XmlNode Group = CreateChildNode(InParent, "ItemGroup");

        for (int i = 0; i < SourceCodeFiles.Count; ++i)
        {
            string CodeRelativePath = SourceDirectory.GetRelativePath(SourceCodeFiles[i]);
            string RelativePath = Path.GetDirectoryName(CodeRelativePath);

            XmlNode ClCompile = CreateChildNode(Group, "ClCompile");
            CreateKeyStringAttribute(ClCompile, "Include", SourceCodeFiles[i].FullPath);
            CreateChildNode(ClCompile, "Filter").InnerText = RelativePath;
        }
    }

    /// <summary>
    /// 생성된 디렉토리 목록을 기반으로 필터를 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    private void GenerateFilters(XmlNode InParent)
    {
        XmlNode Group = CreateChildNode(InParent, "ItemGroup");
        GenerateFiltersRecursive(Group, SourceDirectory.GetCurrentDirectories());
    }

    /// <summary>
    /// 재귀적으로 모든 하위 디렉토리를 필터로 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <param name="Directories"> 현재 단계의 디렉토리 목록을 전달합니다. </param>
    private void GenerateFiltersRecursive(XmlNode InParent, IList<DirectoryReference> Directories)
    {
        List<DirectoryReference> Subdirectories = new();

        for (int i = 0; i < Directories.Count; ++i)
        {
            string RelativePath = SourceDirectory.GetRelativePath(Directories[i]);

            XmlNode Filter = CreateChildNode(InParent, "Filter");
            CreateKeyStringAttribute(Filter, "Include", RelativePath);
            CreateChildNode(Filter, "UniqueIdentifier").InnerText = $"{{{Guid.NewGuid()}}}";

            // 하위 디렉토리를 목록에 추가하여 상위 디렉토리가 먼저 빌드된 후에
            // 서브 디렉토리가 빌드될 수 있도록 합니다.
            foreach (DirectoryReference Subdirectory in Directories[i].GetCurrentDirectories())
            {
                Subdirectories.Add(Subdirectory);
            }
        }

        if (Subdirectories.Count != 0)
        {
            GenerateFiltersRecursive(InParent, Subdirectories);
        }
    }

    /// <summary>
    /// 헤더 파일 포함을 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GenerateHeaderFiles(XmlNode InParent)
    {
        XmlNode Group = CreateChildNode(InParent, "ItemGroup");
        for (int i = 0; i < HeaderFiles.Count; ++i)
        {
            CreateKeyStringAttribute(CreateChildNode(Group, "ClInclude"), "Include", HeaderFiles[i].FullPath);
        }

        return Group;
    }

    /// <summary>
    /// FX 소스 파일 포함을 생성합니다.
    /// </summary>
    /// <param name="MyBuild"> 이 프로젝트의 빌드 정보를 전달합니다. </param>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GenerateFxCodeFiles(XmlNode InParent, CppProjectBuildInfo MyBuild)
    {
        XmlNode Group = CreateChildNode(InParent, "ItemGroup");
        for (int i = 0; i < FxCodeFiles.Count; ++i)
        {
            FileReference File = FxCodeFiles[i];

            XmlNode FxCompile = CreateChildNode(Group, "FxCompile");
            CreateKeyStringAttribute(FxCompile, "Include", FxCodeFiles[i].FullPath);

            string ActualName = Path.GetFileNameWithoutExtension(File.Name);
            string MinorExtension = Path.GetExtension(File.Name);

            if (MinorExtension == ".ShaderLibrary")
            {
                CreateChildNode(FxCompile, "ShaderType").InnerText = "Library";
                CreateChildNode(FxCompile, "ShaderModel").InnerText = "6.3";
                CreateChildNode(FxCompile, "EntryPointName");
            }
            else
            {
                string ShaderType = MinorExtension switch
                {
                    ".VertexShader" => "Vertex",
                    ".PixelShader" => "Pixel",
                    ".DomainShader" => "Domain",
                    ".HullShader" => "Hull",
                    ".GeometryShader" => "Geomtry",
                    ".ComputeShader" => "Compute",
                    _ => throw new ArgumentException("파일 이름이 잘못되었습니다. 확장자가 .hlsl인 Fx 파일은 중간 확장자로 셰이더 형식을 지정하여야 합니다.")
                };

                CreateChildNode(FxCompile, "ShaderType").InnerText = ShaderType;
                CreateChildNode(FxCompile, "ShaderModel").InnerText = "5.1";
                CreateChildNode(FxCompile, "EntryPointName").InnerText = "Main";
            }

            // 파일의 상대 경로를 구합니다.
            string RelativePath = SourceDirectory.GetRelativePath(File.GetParent());

            // Public, Private 구분자를 제거합니다.
            int IndexOfFirstDirectory = RelativePath.IndexOf("\\");
            if (IndexOfFirstDirectory != -1)
            {
                RelativePath = RelativePath.Remove(0, IndexOfFirstDirectory + 1);
            }
            CreateChildNode(FxCompile, "VariableName").InnerText = $"p{ActualName}{MinorExtension.Substring(1)}";
            CreateChildNode(FxCompile, "HeaderFileOutput").InnerText = $"$(IntDir)CompiledShaders\\{RelativePath}\\{File.Name}.generated.h";
        }

        return Group;
    }

    /// <summary>
    /// 소스 파일 포함을 생성합니다.
    /// </summary>
    /// <param name="MyBuild"> 이 프로젝트의 빌드 정보를 전달합니다. </param>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GenerateSourceFiles(XmlNode InParent, CppProjectBuildInfo MyBuild)
    {
        XmlNode Group = CreateChildNode(InParent, "ItemGroup");
        for (int i = 0; i < SourceCodeFiles.Count; ++i)
        {
            XmlNode ClCompile = CreateChildNode(Group, "ClCompile");
            CreateKeyStringAttribute(ClCompile, "Include", SourceCodeFiles[i].FullPath);

            if (!string.IsNullOrEmpty(MyBuild.Rules.PrivatePCHHeaderFile) && SourceCodeFiles[i].Name.Equals(MyBuild.Rules.PrivatePCHHeaderFile, StringComparison.OrdinalIgnoreCase))
            {
                CreateChildNode(ClCompile, "PrecompiledHeader").InnerText = "Create";
            }
        }

        return Group;
    }

    /// <summary>
    /// 컴파일 아이템 그룹을 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <param name="MyBuild"> 이 프로젝트의 빌드 정보를 전달합니다. </param>
    /// <param name="GeneratedDependencyProjects"> 의존 관계 프로젝트 목록을 전달합니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GenerateItemDefinitionGroup(XmlNode InParent, CppProjectBuildInfo MyBuild, List<CppProjectBuildInfo> GeneratedDependencyProjects)
    {
        List<string> AdditionalIncludePaths = new()
        {
             $"{SourceDirectory.FullPath}\\Public",
             $"{SourceDirectory.FullPath}\\Private",
             $"$(IntDir)CompiledShaders\\",
        };

        foreach (var Item in GeneratedDependencyProjects)
        {
            AdditionalIncludePaths.Add($"{Item.SourceRoot}\\Public");
        }

        XmlNode Group = CreateChildNode(InParent, "ItemDefinitionGroup");

        // C++ 코드 컴파일 노드입니다.
        XmlNode ClCompile = CreateChildNode(Group, "ClCompile");
        CreateChildNode(ClCompile, "WarningLevel").InnerText = "Level3";
        CreateChildNode(ClCompile, "SDKCheck").InnerText = "true";
        CreateChildNode(ClCompile, "ConformanceMode").InnerText = "true";
        CreateChildNode(ClCompile, "AdditionalIncludeDirectories").InnerText = $"{string.Join(';', AdditionalIncludePaths)};%(AdditionalIncludeDirectories)";

        if (!string.IsNullOrEmpty(MyBuild.Rules.PrivatePCHHeaderFile))
        {
            CreateChildNode(ClCompile, "PrecompiledHeader").InnerText = "Use";
            CreateChildNode(ClCompile, "PrecompiledHeaderFile").InnerText = $"{MyBuild.Rules.PrivatePCHHeaderFile}.h";
            CreateChildNode(ClCompile, "ForcedIncludeFiles").InnerText = $"{MyBuild.Rules.PrivatePCHHeaderFile}.h";
        }
        CreateChildNode(ClCompile, "MultiProcessorCompilation").InnerText = "true";
        CreateChildNode(ClCompile, "LanguageStandard").InnerText = MyBuild.Rules.CppStandardVersion switch
        {
            CppVersion.Cpp14 => "stdcpp14",
            CppVersion.Cpp17 => "stdcpp17",
            CppVersion.Cpp20 => "stdcpplatest",
            _ => "Default"
        };

        // HLSL 컴파일 노드입니다.
        XmlNode FxCompile = CreateChildNode(Group, "FxCompile");
        CreateChildNode(FxCompile, "FileType").InnerText = "Document";
        CreateChildNode(FxCompile, "ObjectFileOutput");

        // 링크 노드입니다.
        XmlNode Link = CreateChildNode(Group, "Link");
        CreateChildNode(Link, "SubSystem").InnerText = "Windows";
        CreateChildNode(Link, "GenerateDebugInformation").InnerText = "true";
        CreateChildNode(Link, "EnableUAC").InnerText = "false";
        if (MyBuild.Rules.AdditionalDependencies.Count != 0)
        {
            CreateChildNode(Link, "AdditionalDependencies").InnerText = $"{string.Join(';', MyBuild.Rules.AdditionalDependencies)};kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)";
        }

        // 빌드 후 이벤트 노드입니다.
        XmlNode PostBuildEvent = CreateChildNode(Group, "PostBuildEvent");
        StringBuilder PostBuildEventBuilder = new();

        foreach (CppProjectBuildInfo Item in GeneratedDependencyProjects)
        {
            if (Item.Rules.bDirectLink)
            {
                PostBuildEventBuilder.AppendLine($"xcopy \"{Item.SourceRoot}\\Binaries\\\" \"$(OutDir)\" /Y");
            }
        }

        CreateChildNode(PostBuildEvent, "Command").InnerText = PostBuildEventBuilder.ToString();

        return Group;
    }

    /// <summary>
    /// 컴파일 아이템 그룹을 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <param name="GeneratedDependencyProjects"> 의존 관계 프로젝트 목록을 전달합니다. </param>
    /// <param name="bRelease"> 릴리즈 모드로 생성합니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GenerateItemDefinitionGroup(XmlNode InParent, bool bRelease, CppProjectBuildInfo MyBuild, List<CppProjectBuildInfo> GeneratedDependencyProjects)
    {
        List<string> AdditionalPreprocessorDefines = new()
        {
            $"{ProjectName.ToUpper()}_API=__declspec(dllexport)",
            $"{ProjectName.ToUpper()}_BUILD",
        };

        AdditionalPreprocessorDefines.AddRange(MyBuild.Rules.AdditionalPreprocessorDefines);

        foreach (var Item in GeneratedDependencyProjects)
        {
            AdditionalPreprocessorDefines.Add($"{Item.Symbol.ToUpper()}_API=__declspec(dllimport)");
        }

        string AdditionalPreprocessorDefine = string.Join(';', AdditionalPreprocessorDefines);

        string Configuration = bRelease ? "Release" : "Debug";
        string Platform = "x64";
        string ReleaseCaseString = bRelease.ToString().ToLower();
        string PreprocessorDefinitions = bRelease
            ? $"{AdditionalPreprocessorDefine};WITH_DEBUG=0;_WINDOWS;_USRDLL;%(PreprocessorDefinitions)"
            : $"{AdditionalPreprocessorDefine};WITH_DEBUG=1;_WINDOWS;_USRDLL;%(PreprocessorDefinitions)";

        XmlNode Group = CreateChildNode(InParent, "ItemDefinitionGroup");
        CreateKeyStringAttribute(Group, "Condition", $"'$(Configuration)|$(Platform)'=='{Configuration}|{Platform}'");
        XmlNode ClCompile = CreateChildNode(Group, "ClCompile");
        CreateChildNode(ClCompile, "FunctionLevelLinking").InnerText = ReleaseCaseString;
        CreateChildNode(ClCompile, "IntrinsicFunctions").InnerText = ReleaseCaseString;
        CreateChildNode(ClCompile, "PreprocessorDefinitions").InnerText = PreprocessorDefinitions;
        XmlNode Link = CreateChildNode(Group, "Link");
        CreateChildNode(Link, "EnableCOMDATFolding").InnerText = ReleaseCaseString;
        CreateChildNode(Link, "OptimizeReferences").InnerText = ReleaseCaseString;
        return Group;
    }

    /// <summary>
    /// 프로퍼티 그룹을 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <param name="bLaunch"> 프로그램 실행을 위한 프로젝트입니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GeneratePropertySheets(XmlNode InParent, bool bLaunch)
    {
        string ConfigurationType = bLaunch
            ? "Application"
            : "DynamicLibrary";

        XmlNode PropertyGroup = CreateChildNode(InParent, "PropertyGroup");
        CreateChildNode(PropertyGroup, "LinkIncremental").InnerText = "false";
        CreateChildNode(PropertyGroup, "OutDir").InnerText = "$(SolutionDir)Build\\$(Platform)\\";
        CreateChildNode(PropertyGroup, "IntDir").InnerText = "$(SolutionDir)Intermediate\\Build\\$(Platform)\\$(ProjectName)\\";
        CreateChildNode(PropertyGroup, "ConfigurationType").InnerText = ConfigurationType;
        CreateChildNode(PropertyGroup, "PlatformToolset").InnerText = PlatformToolset[CppProjectGeneratorVersion.VC2019];
        CreateChildNode(PropertyGroup, "CharacterSet").InnerText = "Unicode";

        return PropertyGroup;
    }

    /// <summary>
    /// 프로퍼티 그룹 시트를 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <param name="bRelease"> 릴리즈 모드로 생성합니다. </param>
    /// <param name="GeneratedDependencyProjects"> 의존 관계 프로젝트 목록을 전달합니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GeneratePropertyGroup(XmlNode InParent, bool bRelease, List<CppProjectBuildInfo> GeneratedDependencyProjects)
    {
        string Configuration = bRelease ? "Release" : "Debug";
        string Platform = "x64";

        XmlNode MyNode = CreateChildNode(InParent, "PropertyGroup");
        CreateKeyStringAttribute(MyNode, "Condition", $"'$(Configuration)|$(Platform)'=='{Configuration}|{Platform}'");
        CreateChildNode(MyNode, "UseDebugLibrary").InnerText = "false";

        if (bRelease)
        {
            CreateChildNode(MyNode, "WholeProgramOptimization").InnerText = "true";
        }

        List<string> LibraryPaths = new();
        foreach (CppProjectBuildInfo Item in GeneratedDependencyProjects)
        {
            if (Item.Rules.bDirectLink)
            {
                LibraryPaths.Add(Item.SourceRoot.Move("Libraries").ToString());
            }
        }

        if (LibraryPaths.Count != 0)
        {
            LibraryPaths.Add("$(LibraryPath)");
            CreateChildNode(MyNode, "LibraryPath").InnerText = string.Join(';', LibraryPaths);
        }

        return MyNode;
    }

    /// <summary>
    /// Import 그룹 노드를 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <param name="Label"> 라벨 이름을 전달합니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GenerateImportGroup(XmlNode InParent, string Label)
    {
        XmlNode MyNode = CreateChildNode(InParent, "ImportGroup");
        CreateKeyStringAttribute(MyNode, "Label", Label);
        return MyNode;
    }

    /// <summary>
    /// 프로젝트 Import 노드를 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <param name="Import"> 임포트 프로젝트를 전달합니다. </param>
    /// <param name="Label"> 라벨 이름을 전달합니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GenerateImportProject(XmlNode InParent, string Import, string Label = null)
    {
        XmlNode MyNode = CreateChildNode(InParent, "Import");
        CreateKeyStringAttribute(MyNode, "Project", Import);
        if (Label is not null)
        {
            CreateKeyStringAttribute(MyNode, "Condition", $"exists('{Import}')");
            CreateKeyStringAttribute(MyNode, "Label", Label);
        }
        return MyNode;
    }

    /// <summary>
    /// 전역 프로퍼티 그룹 시트를 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GeneratePropertyGroupsGlobal(XmlNode InParent)
    {
        XmlNode MyNode = CreateChildNode(InParent, "PropertyGroup");
        CreateKeyStringAttribute(MyNode, "Label", "Globals");

        CreateChildNode(MyNode, "VCProjectVersion").InnerText = VCProjectVersion[CppProjectGeneratorVersion.VC2019];
        CreateChildNode(MyNode, "Keyword").InnerText = "Win32Proj";
        CreateChildNode(MyNode, "ProjectGuid").InnerText = $"{{{Guid.NewGuid().ToString()}}}";
        CreateChildNode(MyNode, "WindowsTargetPlatformVersion").InnerText = "10.0";

        return MyNode;
    }

    /// <summary>
    /// ItemGroup 노드를 생성합니다.
    /// </summary>
    /// <param name="InParent"> 상위 노드를 전달합니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GenerateItemGroupNode(XmlNode InParent)
    {
        XmlNode MyNode = CreateChildNode(InParent, "ItemGroup");
        CreateKeyStringAttribute(MyNode, "Label", "ProjectConfigurations");

        XmlNode ProjectConfiguration = CreateChildNode(MyNode, "ProjectConfiguration");
        CreateKeyStringAttribute(ProjectConfiguration, "Include", "Debug|x64");

        XmlNode Configuration = CreateChildNode(ProjectConfiguration, "Configuration");
        Configuration.InnerText = "Debug";
        XmlNode Platform = CreateChildNode(ProjectConfiguration, "Platform");
        Platform.InnerText = "x64";

        ProjectConfiguration = CreateChildNode(MyNode, "ProjectConfiguration");
        CreateKeyStringAttribute(ProjectConfiguration, "Include", "Release|x64");

        Configuration = CreateChildNode(ProjectConfiguration, "Configuration");
        Configuration.InnerText = "Release";
        Platform = CreateChildNode(ProjectConfiguration, "Platform");
        Platform.InnerText = "x64";

        return MyNode;
    }

    /// <summary>
    /// 프로젝트 루트 노드를 생성합니다.
    /// </summary>
    /// <param name="InDoc"> Xml 문서를 전달합니다. </param>
    /// <returns> 생성된 노드가 반환됩니다. </returns>
    private XmlNode GenerateProjectNode(XmlDocument InDoc, bool bFilter = false)
    {
        XmlNode MyNode = InDoc.CreateElement("Project");
        InDoc.AppendChild(MyNode);
        if (bFilter)
        {
            CreateKeyStringAttribute(MyNode, "ToolsVersion", "4.0");
        }
        else
        {
            CreateKeyStringAttribute(MyNode, "DefaultTargets", "Build");
        }
        CreateKeyStringAttribute(MyNode, "xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");
        return MyNode;
    }

    private XmlNode CreateChildNode(XmlNode InParent, string NodeName)
    {
        XmlDocument MyDoc = InParent.OwnerDocument;
        XmlNode MyNode = MyDoc.CreateElement(NodeName);
        InParent.AppendChild(MyNode);
        return MyNode;
    }

    /// <summary>
    /// Key-Value 쌍으로 이루어진 어트리뷰트를 생성합니다. 값은 쌍따옴표로 묶입니다.
    /// </summary>
    /// <param name="InParent"> 어트리뷰트 소유자를 전달합니다. </param>
    /// <param name="Key"> 키를 전달합니다. </param>
    /// <param name="Value"> 값을 전달합니다. </param>
    private void CreateKeyStringAttribute(XmlNode InParent, string Key, string Value)
    {
        XmlDocument MyDoc = InParent.OwnerDocument;
        XmlAttribute Attr = MyDoc.CreateAttribute(Key);
        Attr.Value = Value;

        InParent.Attributes.Append(Attr);
    }
}