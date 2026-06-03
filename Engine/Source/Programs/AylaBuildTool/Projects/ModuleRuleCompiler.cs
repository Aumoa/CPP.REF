using System.Reflection;
using Microsoft.CodeAnalysis;

namespace AylaEngine;

internal sealed class ModuleRuleCompiler
{
    private static readonly CSProject ProjectTemplate = new("Microsoft.NET.Sdk",
        [new CSPropertyGroup(
            null,
            OutputKind.DynamicallyLinkedLibrary,
            CSTargetFramework.Net0900,
            true,
            NullableContextOptions.Enable,
            null,
            null,
            null,
            null,
            null,
            null,
            null,
            [],
            [],
            null,
            null,
            [],
            null
            )],
        [new CSItemGroup(
            null,
            [new CSFileReference("AylaBuildRules", typeof(ModuleRules).Assembly.Location)],
            [],
            []
            )],
        null);

    private readonly ModuleRuleCache m_Cache;

    public ModuleRuleCompiler(ModuleRuleCache cache)
    {
        m_Cache = cache;
    }

    public async Task<Type> GetRuleTypeAsync(
        string moduleName,
        string moduleDirectory,
        GroupDescriptor group,
        string ruleFileName,
        CancellationToken cancellationToken)
    {
        var cacheEntry = m_Cache.GetEntry(group, ruleFileName);
        var assembly = m_Cache.LoadIfValid(cacheEntry);
        if (assembly == null)
        {
            assembly = await CompileAndLoadAsync(moduleDirectory, cacheEntry, cancellationToken);
        }

        var className = moduleName.Replace('.', '_');
        return assembly.GetTypes().First(p => p.Name == className);
    }

    private async Task<Assembly> CompileAndLoadAsync(
        string moduleDirectory,
        ModuleRuleCacheEntry cacheEntry,
        CancellationToken cancellationToken)
    {
        var projectName = Path.GetFileNameWithoutExtension(cacheEntry.AssemblyFileName);
        var project = ProjectTemplate with
        {
            PropertyGroups = [ProjectTemplate.PropertyGroup with
            {
                OutputPath = Path.GetDirectoryName(cacheEntry.AssemblyFileName),
                AssemblyName = projectName
            }]
        };

        CSSourceCode[] sourceCodes = [CSSourceCode.FromFile(cacheEntry.RuleFileName)];

        try
        {
            await CSCompiler.CompileAsAsync(sourceCodes, project, [], moduleDirectory, projectName, cancellationToken);
            await m_Cache.SaveAsync(cacheEntry, cancellationToken);
            return await Task.Run(() => Assembly.LoadFile(cacheEntry.AssemblyFileName), cancellationToken);
        }
        catch (CSCompilerError e)
        {
            Console.Error.WriteLine(e.Message);
            throw TerminateException.User();
        }
    }
}
