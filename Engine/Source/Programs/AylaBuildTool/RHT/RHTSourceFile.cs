namespace AylaEngine;

internal readonly record struct RHTSourceFile(
    string FilePath,
    string SourceRootDirectory,
    string ModuleName);
