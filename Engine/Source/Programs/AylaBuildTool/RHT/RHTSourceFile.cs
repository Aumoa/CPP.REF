namespace AylaEngine.RHT;

internal readonly record struct RHTSourceFile(
    string FilePath,
    string SourceRootDirectory,
    string ModuleName);
