namespace AylaEngine;

internal sealed class CppCompileCommand
{
    private readonly CppCompileEnvironment m_Environment;
    private readonly SourceCodeDescriptor m_SourceCode;
    private readonly CppPchCommandKind m_PchCommandKind;
    private readonly CppPchSettings? m_PchSettings;
    private readonly string m_FileName;
    private readonly string m_OutputName;
    private readonly string m_ObjectFilePath;
    private readonly string m_PdbFilePath;
    private readonly string m_DependenciesFilePath;
    private readonly string m_CacheFilePath;

    public CppCompileCommand(CppCompileEnvironment environment, SourceCodeDescriptor sourceCode)
        : this(environment, sourceCode, Path.GetFileName(sourceCode.FilePath), GetDefaultPchCommandKind(environment, sourceCode))
    {
    }

    private CppCompileCommand(CppCompileEnvironment environment, SourceCodeDescriptor sourceCode, string outputName, CppPchCommandKind pchCommandKind)
    {
        m_Environment = environment;
        m_SourceCode = sourceCode;
        m_PchCommandKind = pchCommandKind;
        m_PchSettings = environment.PchSettings;
        m_FileName = Path.GetFileName(sourceCode.FilePath);
        m_OutputName = outputName;
        m_ObjectFilePath = Path.Combine(environment.IntermediateDirectory, m_OutputName + ".o");
        m_PdbFilePath = Path.Combine(environment.IntermediateDirectory, m_OutputName + ".pdb");
        m_DependenciesFilePath = Path.Combine(environment.IntermediateDirectory, m_OutputName + ".deps");
        m_CacheFilePath = Path.Combine(environment.IntermediateDirectory, m_OutputName + ".cache");
    }

    public CppCompileEnvironment Environment => m_Environment;

    public ModuleRulesResolver Resolver => m_Environment.Resolver;

    public SourceCodeDescriptor SourceCode => m_SourceCode;

    public CppPchCommandKind PchCommandKind => m_PchCommandKind;

    public CppPchSettings? PchSettings => m_PchSettings;

    public GroupDescriptor Descriptor => m_Environment.Descriptor;

    public string IntermediateDirectory => m_Environment.IntermediateDirectory;

    public string FileName => m_FileName;

    public string OutputName => m_OutputName;

    public string ObjectFilePath => m_ObjectFilePath;

    public string PdbFilePath => m_PdbFilePath;

    public string DependenciesFilePath => m_DependenciesFilePath;

    public string CacheFilePath => m_CacheFilePath;

    public bool IsModuleInterface => m_SourceCode.Type == SourceCodeType.ModuleInterface;

    public bool CreatesPch => m_PchCommandKind == CppPchCommandKind.Create;

    public bool UsesPch => m_PchCommandKind == CppPchCommandKind.Use;

    public static CppCompileCommand CreatePch(CppCompileEnvironment environment)
    {
        var pchSettings = environment.PchSettings
            ?? throw new InvalidOperationException("The compile environment does not have PCH settings.");

        var sourceCode = new SourceCodeDescriptor(
            environment.Descriptor,
            environment.Resolver.Name,
            pchSettings.SourceFilePath,
            Path.GetFileName(pchSettings.SourceFilePath),
            SourceCodeType.SourceCode);

        return new CppCompileCommand(environment, sourceCode, pchSettings.OutputName, CppPchCommandKind.Create);
    }

    private static CppPchCommandKind GetDefaultPchCommandKind(CppCompileEnvironment environment, SourceCodeDescriptor sourceCode)
    {
        if (environment.PchSettings == null)
        {
            return CppPchCommandKind.None;
        }

        if (sourceCode.Type != SourceCodeType.SourceCode)
        {
            return CppPchCommandKind.None;
        }

        return CppPchCommandKind.Use;
    }
}
