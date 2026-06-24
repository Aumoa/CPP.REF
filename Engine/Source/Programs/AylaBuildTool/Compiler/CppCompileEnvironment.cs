namespace AylaEngine;

internal sealed class CppCompileEnvironment
{
    private readonly ModuleRulesResolver m_Resolver;
    private readonly TargetInfo m_TargetInfo;
    private readonly GroupDescriptor m_Descriptor;
    private readonly string m_IntermediateDirectory;
    private readonly PchUsageMode m_PchUsage;
    private readonly string? m_PrivatePchHeaderFile;
    private readonly CppPchSettings? m_PchSettings;
    private readonly string[] m_IncludePaths;
    private readonly MacroSet[] m_AdditionalMacros;
    private readonly int[] m_DisableWarnings;

    public CppCompileEnvironment(ModuleRulesResolver resolver, TargetInfo targetInfo, GroupDescriptor descriptor)
    {
        m_Resolver = resolver;
        m_TargetInfo = targetInfo;
        m_Descriptor = descriptor;
        m_IntermediateDirectory = descriptor.Intermediate(resolver.Name, targetInfo, resolver.BuildProfile, FolderPolicy.PathType.Current);
        m_PchUsage = resolver.PchUsage;
        m_PrivatePchHeaderFile = resolver.PrivatePchHeaderFile;
        m_PchSettings = CppPchSettings.CreateOrNull(this);
        m_IncludePaths = resolver.IncludePaths.ToArray();
        m_AdditionalMacros = resolver.AdditionalMacros.ToArray();
        m_DisableWarnings = resolver.DisableWarnings.ToArray();
    }

    public ModuleRulesResolver Resolver => m_Resolver;

    public TargetInfo TargetInfo => m_TargetInfo;

    public GroupDescriptor Descriptor => m_Descriptor;

    public string IntermediateDirectory => m_IntermediateDirectory;

    public PchUsageMode PchUsage => m_PchUsage;

    public string? PrivatePchHeaderFile => m_PrivatePchHeaderFile;

    public CppPchSettings? PchSettings => m_PchSettings;

    public IReadOnlyList<string> IncludePaths => m_IncludePaths;

    public IReadOnlyList<MacroSet> AdditionalMacros => m_AdditionalMacros;

    public IReadOnlyList<int> DisableWarnings => m_DisableWarnings;
}
