namespace AylaEngine;

internal readonly record struct ModuleRuleCacheEntry(
    string RuleFileName,
    string AssemblyFileName,
    string CacheFileName);
