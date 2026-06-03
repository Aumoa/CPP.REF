using System.Reflection;

namespace AylaEngine;

internal sealed class ModuleRuleCache
{
    public ModuleRuleCacheEntry GetEntry(GroupDescriptor group, string ruleFileName)
    {
        var fileName = Path.GetFileName(ruleFileName).Replace('.', '_');
        var directoryName = Path.Combine(group.IntermediateDirectory, "Rules");
        Directory.CreateDirectory(directoryName);

        return new ModuleRuleCacheEntry(
            ruleFileName,
            Path.GetFullPath(Path.Combine(directoryName, fileName + ".dll")),
            Path.GetFullPath(Path.Combine(directoryName, fileName + ".cache")));
    }

    public Assembly? LoadIfValid(ModuleRuleCacheEntry entry)
    {
        if (File.Exists(entry.CacheFileName) == false)
        {
            return null;
        }

        if (File.Exists(entry.AssemblyFileName) == false)
        {
            return null;
        }

        var currentCacheText = CreateCacheText(entry.RuleFileName);
        var cachedText = File.ReadAllText(entry.CacheFileName);
        if (currentCacheText != cachedText)
        {
            return null;
        }

        return Assembly.LoadFile(entry.AssemblyFileName);
    }

    public async Task SaveAsync(ModuleRuleCacheEntry entry, CancellationToken cancellationToken)
    {
        await File.WriteAllTextAsync(entry.CacheFileName, CreateCacheText(entry.RuleFileName), cancellationToken);
    }

    private static string CreateCacheText(string ruleFileName)
    {
        var rulesAssemblyFileName = typeof(ModuleRules).Assembly.Location;
        var rulesAssemblyWriteTime = File.GetLastWriteTimeUtc(rulesAssemblyFileName).ToBinary();
        var ruleText = File.ReadAllText(ruleFileName);

        return string.Join('\n',
        [
            "AylaBuildRulesCacheVersion=1",
            $"RulesAssembly={rulesAssemblyFileName}",
            $"RulesAssemblyWriteTimeUtc={rulesAssemblyWriteTime}",
            ruleText
        ]);
    }
}
