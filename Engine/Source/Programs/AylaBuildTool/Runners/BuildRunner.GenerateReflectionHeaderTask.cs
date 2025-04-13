using System.Threading;
using System;

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

        public RHTGenerator? Generator { get; private set; }

        public async Task<GenerateReflectionHeaderTask> ParseAsync(CancellationToken cancellationToken)
        {
            try
            {
                Generator = await RHTGenerator.ParseAsync(m_SourceCode, cancellationToken);
            }
            catch (Exception e)
            {
                ErrorText = e.Message;
            }

            return this;
        }

        public async Task<bool> TryGenerateAsync(RHTGenerator.Collection collection, TargetInfo targetInfo, CancellationToken cancellationToken = default)
        {
            if (Generator == null)
            {
                return false;
            }

            var fileName = Path.GetFileNameWithoutExtension(m_SourceCode.FilePath);
            var intDir = Project.Descriptor.Intermediate(Project.Name, targetInfo, FolderPolicy.PathType.Current);
            var generatedHeader = Path.Combine(intDir, fileName + ".gen.h");
            var generatedSourceCode = Path.Combine(intDir, fileName + ".gen.cpp");
            var generatedBindingCode = Path.Combine(intDir, "Bindings", fileName + ".bindings.cs");

            Directory.CreateDirectory(Path.Combine(intDir, "Bindings"));
            var headerText = Generator.GenerateHeader().Replace("\r\n", "\n").Trim();
            await TextFileHelper.WriteIfChangedAsync(generatedHeader, headerText, cancellationToken);

            var sourceCodeText = Generator.GenerateSourceCode(collection).Replace("\r\n", "\n").Trim();
            await TextFileHelper.WriteIfChangedAsync(generatedSourceCode, sourceCodeText, cancellationToken);

            GeneratedSourceCode = SourceCodeDescriptor.Get(Project.Descriptor, Project.Name, generatedSourceCode, Project.Descriptor.IntermediateDirectory);

            if (Project.GetRule(targetInfo).DisableGenerateBindings == false)
            {
                var bindingCodeText = Generator.GenerateBindings().Replace("\r\n", "\n").Trim();
                await TextFileHelper.WriteIfChangedAsync(generatedBindingCode, bindingCodeText, cancellationToken);

                GeneratedBindingCode = generatedBindingCode;
            }

            return true;
        }
    }
}
