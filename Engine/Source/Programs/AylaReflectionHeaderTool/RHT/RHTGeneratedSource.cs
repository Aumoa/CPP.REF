namespace AylaEngine.RHT;

public readonly record struct RHTGeneratedSource(
    RHTGenerationInput Input,
    string HeaderText,
    string SourceCodeText,
    string CSharpText);
