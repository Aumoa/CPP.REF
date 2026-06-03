namespace AylaEngine.RHT;

public readonly record struct RHTSourceFile(
    string FilePath,
    string SourceRootDirectory,
    string ModuleName);
