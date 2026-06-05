namespace AylaEngine;

internal sealed class CppCompileCommand
{
    private readonly CppCompileEnvironment m_Environment;
    private readonly SourceCodeDescriptor m_SourceCode;
    private readonly string m_FileName;
    private readonly string m_ObjectFilePath;
    private readonly string m_PdbFilePath;
    private readonly string m_DependenciesFilePath;
    private readonly string m_CacheFilePath;

    public CppCompileCommand(CppCompileEnvironment environment, SourceCodeDescriptor sourceCode)
    {
        m_Environment = environment;
        m_SourceCode = sourceCode;
        m_FileName = Path.GetFileName(sourceCode.FilePath);
        m_ObjectFilePath = Path.Combine(environment.IntermediateDirectory, m_FileName + ".o");
        m_PdbFilePath = Path.Combine(environment.IntermediateDirectory, m_FileName + ".pdb");
        m_DependenciesFilePath = Path.Combine(environment.IntermediateDirectory, m_FileName + ".deps");
        m_CacheFilePath = Path.Combine(environment.IntermediateDirectory, m_FileName + ".cache");
    }

    public CppCompileEnvironment Environment => m_Environment;

    public ModuleRulesResolver Resolver => m_Environment.Resolver;

    public SourceCodeDescriptor SourceCode => m_SourceCode;

    public GroupDescriptor Descriptor => m_Environment.Descriptor;

    public string IntermediateDirectory => m_Environment.IntermediateDirectory;

    public string FileName => m_FileName;

    public string ObjectFilePath => m_ObjectFilePath;

    public string PdbFilePath => m_PdbFilePath;

    public string DependenciesFilePath => m_DependenciesFilePath;

    public string CacheFilePath => m_CacheFilePath;

    public bool IsModuleInterface => m_SourceCode.Type == SourceCodeType.ModuleInterface;
}
