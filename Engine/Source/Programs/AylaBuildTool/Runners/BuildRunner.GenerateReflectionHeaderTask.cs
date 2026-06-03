namespace AylaEngine;

internal static partial class BuildRunner
{
    private class GenerateReflectionHeaderTask
    {
        public readonly ModuleProject Project;
        public readonly TargetInfo BuildTarget;
        private readonly SourceCodeDescriptor m_SourceCode;

        public GenerateReflectionHeaderTask(ModuleProject project, TargetInfo buildTarget, SourceCodeDescriptor sourceCode)
        {
            Project = project;
            BuildTarget = buildTarget;
            m_SourceCode = sourceCode;
        }

        public SourceCodeDescriptor? GeneratedSourceCode { get; private set; }

        public string? ErrorText { get; private set; }

        public RHTGenerator? Generator { get; private set; }

        public async Task<GenerateReflectionHeaderTask> ParseAsync(CancellationToken cancellationToken)
        {
            try
            {
                var sourceFile = new RHTSourceFile(
                    m_SourceCode.FilePath,
                    m_SourceCode.Group.SourceDirectory,
                    m_SourceCode.ModuleName);
                Generator = await RHTGenerator.ParseAsync(sourceFile, cancellationToken);
            }
            catch (Exception e)
            {
                ErrorText = e.Message;
            }

            return this;
        }

        public async Task<bool> TryGenerateAsync(TypeNames collection, TargetInfo targetInfo, CancellationToken cancellationToken = default)
        {
            if (Generator == null)
            {
                return false;
            }

            var fileName = Path.GetFileNameWithoutExtension(m_SourceCode.FilePath);
            var intDir = Project.Group.Intermediate(Project.Name, targetInfo, FolderPolicy.PathType.Current);
            var generatedHeader = Path.Combine(intDir, fileName + ".gen.h");
            var generatedSourceCode = Path.Combine(intDir, fileName + ".gen.cpp");
            var generatedBindingCode = Path.Combine(Project.SourceDirectory, "Script", "Bindings", fileName + ".bindings.cs");

            Directory.CreateDirectory(Path.Combine(Project.SourceDirectory, "Script", "Bindings"));
            var headerText = Generator.GenerateHeader(collection).Replace("\r\n", "\n");
            await TextFileHelper.WriteIfChangedAsync(generatedHeader, headerText, cancellationToken);

            var sourceCodeText = Generator.GenerateSourceCode(Project, BuildTarget, collection).Replace("\r\n", "\n");
            await TextFileHelper.WriteIfChangedAsync(generatedSourceCode, sourceCodeText, cancellationToken);

            var csText = Generator.GenerateCSharp(Project, BuildTarget, collection).Replace("\r\n", "\n");
            await TextFileHelper.WriteIfChangedAsync(generatedBindingCode, csText, cancellationToken);

            GeneratedSourceCode = SourceCodeDescriptor.Get(Project.Group, Project.Name, generatedSourceCode, Project.Group.IntermediateDirectory);
            return true;
        }
    }
}
