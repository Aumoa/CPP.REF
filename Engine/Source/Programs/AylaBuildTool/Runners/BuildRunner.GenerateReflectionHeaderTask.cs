namespace AylaEngine;

internal static partial class BuildRunner
{
    private class GenerateReflectionHeaderTask
    {
        public readonly ModuleProject Project;
        private readonly SourceCodeDescriptor m_SourceCode;

        public GenerateReflectionHeaderTask(ModuleProject project, SourceCodeDescriptor sourceCode)
        {
            Project = project;
            m_SourceCode = sourceCode;
        }

        public SourceCodeDescriptor? GeneratedSourceCode { get; private set; }

        public string? ErrorText { get; private set; }

        public async Task<GenerateReflectionHeaderTask> GenerateAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            try
            {
                var fileName = Path.GetFileNameWithoutExtension(m_SourceCode.FilePath);
                var intDir = Project.Descriptor.Intermediate(Project.Name, targetInfo, FolderPolicy.PathType.Current);
                var generatedHeader = Path.Combine(intDir, fileName + ".gen.h");
                var generatedSourceCode = Path.Combine(intDir, fileName + ".gen.cpp");
                var generator = await RHTGenerator.ParseAsync(m_SourceCode, cancellationToken);
                if (generator != null)
                {
                    var headerText = generator.GenerateHeader().Replace("\r\n", "\n").Trim();
                    await CompareExchangeContentAsync(generatedHeader, headerText, cancellationToken);

                    var sourceCodeText = generator.GenerateSourceCode().Replace("\r\n", "\n").Trim();
                    await CompareExchangeContentAsync(generatedSourceCode, sourceCodeText, cancellationToken);

                    GeneratedSourceCode = SourceCodeDescriptor.Get(Project.Descriptor, generatedSourceCode, Project.Descriptor.IntermediateDirectory);
                }
            }
            catch (Exception e)
            {
                ErrorText = e.Message;
            }

            return this;
        }

        private static async Task CompareExchangeContentAsync(string filePath, string content, CancellationToken cancellationToken)
        {
            if (File.Exists(filePath))
            {
                var previousContent = await File.ReadAllTextAsync(filePath, cancellationToken);
                if (previousContent.Trim().Replace("\r\n", "\n") == content)
                {
                    return;
                }
            }

            Console.WriteLine("{0} is newer.", filePath);
            await File.WriteAllTextAsync(filePath, content.Replace("\n", Environment.NewLine), cancellationToken);
        }
    }
}
