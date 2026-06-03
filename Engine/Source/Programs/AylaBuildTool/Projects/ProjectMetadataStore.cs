namespace AylaEngine;

internal sealed class ProjectMetadataStore
{
    public async Task<Project.Declaration> GetOrCreateProjectDeclarationAsync(string projectFileName, CancellationToken cancellationToken)
    {
        return await GetOrCreateDeclarationAsync(
            projectFileName + ".meta",
            Project.Declaration.New,
            cancellationToken);
    }

    public async Task<ModuleProject.ModuleDeclaration> GetOrCreateModuleDeclarationAsync(string ruleFileName, CancellationToken cancellationToken)
    {
        return await GetOrCreateDeclarationAsync(
            ruleFileName + ".meta",
            ModuleProject.ModuleDeclaration.New,
            cancellationToken);
    }

    private static async Task<TDeclaration> GetOrCreateDeclarationAsync<TDeclaration>(
        string metadataFileName,
        Func<TDeclaration> createDeclaration,
        CancellationToken cancellationToken)
        where TDeclaration : Project.Declaration
    {
        TDeclaration? declaration = null;
        if (File.Exists(metadataFileName))
        {
            declaration = await MetadataHelper.DeserializeFromFileAsync<TDeclaration>(metadataFileName, cancellationToken);
        }

        if (declaration is not { IsValid: true })
        {
            declaration = createDeclaration();
            await MetadataHelper.SerializeToFileAsync(declaration, metadataFileName, cancellationToken);
        }

        return declaration;
    }
}
