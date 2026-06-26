namespace AylaEngine;

internal sealed class CppPchSettings
{
    private readonly string m_HeaderIncludeName;
    private readonly string m_OutputName;
    private readonly string m_SourceFilePath;
    private readonly string m_PchFilePath;
    private readonly string m_PdbFilePath;
    private readonly string m_CacheFilePath;

    private CppPchSettings(CppCompileEnvironment environment, string headerIncludeName)
    {
        m_HeaderIncludeName = headerIncludeName;
        m_OutputName = environment.Resolver.Name + ".pch";
        m_SourceFilePath = Path.Combine(environment.IntermediateDirectory, m_OutputName + ".cpp");
        m_PchFilePath = Path.Combine(environment.IntermediateDirectory, m_OutputName);
        m_PdbFilePath = Path.Combine(environment.IntermediateDirectory, m_OutputName + ".pdb");
        m_CacheFilePath = Path.Combine(environment.IntermediateDirectory, m_OutputName + ".cache");
    }

    public string HeaderIncludeName => m_HeaderIncludeName;

    public string OutputName => m_OutputName;

    public string SourceFilePath => m_SourceFilePath;

    public string PchFilePath => m_PchFilePath;

    public string PdbFilePath => m_PdbFilePath;

    public string CacheFilePath => m_CacheFilePath;

    public Task WriteSourceFileAsync(CancellationToken cancellationToken)
    {
        var content = $"#include \"{m_HeaderIncludeName}\"\n";
        return TextFileHelper.WriteIfChangedAsync(m_SourceFilePath, content, cancellationToken);
    }

    public static CppPchSettings? CreateOrNull(CppCompileEnvironment environment)
    {
        if (environment.PchUsage != PchUsageMode.UseExplicitOrSharedPCHs)
        {
            return null;
        }

        var headerIncludeName = environment.PrivatePchHeaderFile?.Trim();
        if (string.IsNullOrWhiteSpace(headerIncludeName))
        {
            return null;
        }

        return new CppPchSettings(environment, headerIncludeName);
    }
}
