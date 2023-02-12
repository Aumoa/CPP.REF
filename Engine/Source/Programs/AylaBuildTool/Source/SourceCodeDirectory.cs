// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Source;

public record SourceCodeDirectory
{
    public string RootDirectory { get; init; }

    public string SourceDirectory { get; init; }

    public string IntermediateDirectory { get; init; }

    public string ProjectFilesDirectory { get; init; }

    public string GeneratedDirectory { get; init; }

    public string ContentDirectory { get; init; }

    public string ProgramsDirectory { get; init; }

    public SourceCodeDirectory(string InPath)
    {
        RootDirectory = Path.GetFullPath(InPath);
        SourceDirectory = Path.Combine(RootDirectory, "Source");
        IntermediateDirectory = Path.Combine(RootDirectory, "Intermediate");
        ProjectFilesDirectory = Path.Combine(IntermediateDirectory, "ProjectFiles");
        GeneratedDirectory = Path.Combine(IntermediateDirectory, "Includes");
        ContentDirectory = Path.Combine(RootDirectory, "Content");
        ProgramsDirectory = Path.Combine(SourceDirectory, "Programs");
    }

    public Task GenerateDirectoriesAsync(CancellationToken CToken = default)
    {
        return Task.Run(() =>
        {
            if (Directory.Exists(SourceDirectory) == false && CToken.IsCancellationRequested == false)
            {
                Directory.CreateDirectory(SourceDirectory);
            }
            if (Directory.Exists(IntermediateDirectory) == false && CToken.IsCancellationRequested == false)
            {
                Directory.CreateDirectory(IntermediateDirectory);
            }
            if (Directory.Exists(ProjectFilesDirectory) == false && CToken.IsCancellationRequested == false)
            {
                Directory.CreateDirectory(ProjectFilesDirectory);
            }
            if (Directory.Exists(GeneratedDirectory) == false && CToken.IsCancellationRequested == false)
            {
                Directory.CreateDirectory(GeneratedDirectory);
            }
            if (Directory.Exists(ContentDirectory) == false && CToken.IsCancellationRequested == false)
            {
                Directory.CreateDirectory(ContentDirectory);
            }
        }, CToken);
    }
}
