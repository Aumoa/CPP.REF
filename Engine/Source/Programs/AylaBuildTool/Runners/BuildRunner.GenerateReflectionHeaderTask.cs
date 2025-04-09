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
        
        public string? GeneratedBindingCode { get; private set; }

        public string? ErrorText { get; private set; }

        public async Task<GenerateReflectionHeaderTask> GenerateAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            try
            {
                var fileName = Path.GetFileNameWithoutExtension(m_SourceCode.FilePath);
                var intDir = Project.Descriptor.Intermediate(Project.Name, targetInfo, FolderPolicy.PathType.Current);
                var generatedHeader = Path.Combine(intDir, fileName + ".gen.h");
                var generatedSourceCode = Path.Combine(intDir, fileName + ".gen.cpp");
                var generatedBindingCode = Path.Combine(intDir, "Script", fileName + ".bindings.cs");
                var generator = await RHTGenerator.ParseAsync(m_SourceCode, cancellationToken);
                Directory.CreateDirectory(Path.Combine(intDir, "Script"));
                if (generator != null)
                {
                    var headerText = generator.GenerateHeader().Replace("\r\n", "\n").Trim();
                    await TextFileHelper.WriteIfChangedAsync(generatedHeader, headerText, cancellationToken);

                    var sourceCodeText = generator.GenerateSourceCode().Replace("\r\n", "\n").Trim();
                    await TextFileHelper.WriteIfChangedAsync(generatedSourceCode, sourceCodeText, cancellationToken);

                    var bindingCodeText = generator.GenerateBindings().Replace("\r\n", "\n").Trim();
                    await TextFileHelper.WriteIfChangedAsync(generatedBindingCode, bindingCodeText, cancellationToken);

                    GeneratedSourceCode = SourceCodeDescriptor.Get(Project.Descriptor, Project.Name, generatedSourceCode, Project.Descriptor.IntermediateDirectory);
                    GeneratedBindingCode = generatedBindingCode;
                }
            }
            catch (Exception e)
            {
                ErrorText = e.Message;
            }

            return this;
        }
    }
}
