namespace AylaEngine;

internal static partial class BuildRunner
{
    private class GenerateReflectionHeaderTask
    {
        private readonly ModuleRulesResolver m_Resolver;
        private readonly SourceCodeDescriptor m_SourceCode;

        public GenerateReflectionHeaderTask(ModuleRulesResolver resolver, SourceCodeDescriptor sourceCode)
        {
            m_Resolver = resolver;
            m_SourceCode = sourceCode;
        }

        public async Task<string> GenerateAsync(SemaphoreSlim access, TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            await access.WaitAsync(cancellationToken);
            try
            {
                var fileName = Path.GetFileNameWithoutExtension(m_SourceCode.FilePath);
                var intDir = m_Resolver.Group.Intermediate(m_Resolver.Name, targetInfo, FolderPolicy.PathType.Current);
                var generatedHeader = Path.Combine(intDir, fileName + ".gen.h");
                var generatedSourceCode = Path.Combine(intDir, fileName + ".gen.cpp");
                var generator = await RHTGenerator.ParseAsync(m_SourceCode, cancellationToken);
                if (generator != null)
                {
                    var headerText = generator.GenerateHeader().Trim();
                    await CompareExchangeContentAsync(generatedHeader, headerText, cancellationToken);
                }
                return string.Empty;
            }
            catch (Exception e)
            {
                return e.Message;
            }
            finally
            {
                access.Release();
            }
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

            await File.WriteAllTextAsync(filePath, content.Replace("\r\n", "\n").Trim(), cancellationToken);
        }
    }
}
