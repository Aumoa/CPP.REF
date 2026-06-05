namespace AylaEngine.RHT;

public sealed class RHTGenerationResult
{
    public RHTGenerationResult(IReadOnlyList<RHTGeneratedSource> generatedSources, IReadOnlyList<string> errorTexts)
    {
        GeneratedSources = generatedSources;
        ErrorTexts = errorTexts;
    }

    public IReadOnlyList<RHTGeneratedSource> GeneratedSources { get; }

    public IReadOnlyList<string> ErrorTexts { get; }

    public bool HasErrors => ErrorTexts.Count > 0;
}
