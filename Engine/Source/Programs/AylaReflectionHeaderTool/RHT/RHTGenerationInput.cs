namespace AylaEngine.RHT;

public readonly record struct RHTGenerationInput(
    RHTSourceFile SourceFile,
    RHTGenerationContext Context);
