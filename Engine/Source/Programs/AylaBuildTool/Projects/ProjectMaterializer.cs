namespace AylaEngine;

internal sealed class ProjectMaterializer
{
    private readonly ProjectMetadataStore m_MetadataStore;
    private readonly ModuleRuleCompiler m_ModuleRuleCompiler;

    public ProjectMaterializer(ProjectMetadataStore metadataStore, ModuleRuleCompiler moduleRuleCompiler)
    {
        m_MetadataStore = metadataStore;
        m_ModuleRuleCompiler = moduleRuleCompiler;
    }

    public async Task<IReadOnlyList<Project>> MaterializeAsync(
        IEnumerable<ProjectCandidate> candidates,
        CancellationToken cancellationToken)
    {
        var tasks = candidates.Select(candidate => MaterializeAsync(candidate, cancellationToken));
        return await Task.WhenAll(tasks);
    }

    private async Task<Project> MaterializeAsync(
        ProjectCandidate candidate,
        CancellationToken cancellationToken)
    {
        return candidate.Kind switch
        {
            ProjectCandidateKind.Program => await MaterializeProgramProjectAsync(candidate, cancellationToken),
            ProjectCandidateKind.Module => await MaterializeModuleProjectAsync(candidate, cancellationToken),
            _ => throw new InvalidOperationException($"Unsupported project candidate kind: {candidate.Kind}")
        };
    }

    private async Task<Project> MaterializeProgramProjectAsync(
        ProjectCandidate candidate,
        CancellationToken cancellationToken)
    {
        var declaration = await m_MetadataStore.GetOrCreateProjectDeclarationAsync(
            candidate.DefinitionFileName,
            cancellationToken);

        return new ProgramProject(
            candidate.Name,
            candidate.Group,
            declaration,
            candidate.DefinitionFileName);
    }

    private async Task<Project> MaterializeModuleProjectAsync(
        ProjectCandidate candidate,
        CancellationToken cancellationToken)
    {
        var ruleType = await m_ModuleRuleCompiler.GetRuleTypeAsync(
            candidate.Name,
            candidate.SourceDirectory,
            candidate.Group,
            candidate.DefinitionFileName,
            cancellationToken);
        var declaration = await m_MetadataStore.GetOrCreateModuleDeclarationAsync(
            candidate.DefinitionFileName,
            cancellationToken);

        return new ModuleProject(
            candidate.Name,
            candidate.Group,
            candidate.SourceDirectory,
            ruleType,
            candidate.DefinitionFileName,
            declaration);
    }
}
