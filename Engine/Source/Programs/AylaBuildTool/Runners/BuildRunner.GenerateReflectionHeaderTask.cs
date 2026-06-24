using AylaEngine.RHT;

namespace AylaEngine;

internal static partial class BuildRunner
{
    private class GenerateReflectionHeaderTask
    {
        private readonly ModuleProject m_Project;
        private readonly TargetInfo m_BuildTarget;
        private readonly SourceCodeDescriptor m_SourceCode;

        public GenerateReflectionHeaderTask(ModuleProject project, TargetInfo buildTarget, SourceCodeDescriptor sourceCode)
        {
            m_Project = project;
            m_BuildTarget = buildTarget;
            m_SourceCode = sourceCode;

            var rule = project.GetRule(buildTarget);
            var context = new RHTGenerationContext(
                project.ScriptAssemblyName,
                rule.Type != ModuleType.Application && rule.Type != ModuleType.Console);
            Input = new RHTGenerationInput(
                new RHTSourceFile(
                    sourceCode.FilePath,
                    sourceCode.Group.SourceDirectory,
                    sourceCode.ModuleName),
                context);
        }

        public ModuleProject Project => m_Project;

        public RHTGenerationInput Input { get; }

        public async Task<SourceCodeDescriptor> WriteAsync(RHTGeneratedSource generatedSource, CancellationToken cancellationToken = default)
        {
            var fileName = Path.GetFileNameWithoutExtension(m_SourceCode.FilePath);
            var buildProfile = BuildProfileResolver.Resolve(m_Project, m_BuildTarget);
            var intDir = m_Project.Group.ModuleIntermediate(m_Project.Name, m_BuildTarget, buildProfile, FolderPolicy.PathType.Current);
            var generatedHeader = Path.Combine(intDir, fileName + ".gen.h");
            var generatedSourceCode = Path.Combine(intDir, fileName + ".gen.cpp");
            var generatedBindingCode = Path.Combine(m_Project.SourceDirectory, "Script", "Bindings", fileName + ".bindings.cs");

            Directory.CreateDirectory(Path.Combine(m_Project.SourceDirectory, "Script", "Bindings"));
            var headerText = generatedSource.HeaderText.Replace("\r\n", "\n");
            await TextFileHelper.WriteIfChangedAsync(generatedHeader, headerText, cancellationToken);

            var sourceCodeText = generatedSource.SourceCodeText.Replace("\r\n", "\n");
            await TextFileHelper.WriteIfChangedAsync(generatedSourceCode, sourceCodeText, cancellationToken);

            var csText = generatedSource.CSharpText.Replace("\r\n", "\n");
            await TextFileHelper.WriteIfChangedAsync(generatedBindingCode, csText, cancellationToken);

            return SourceCodeDescriptor.Get(m_Project.Group, m_Project.Name, generatedSourceCode, m_Project.Group.IntermediateDirectory);
        }
    }
}
