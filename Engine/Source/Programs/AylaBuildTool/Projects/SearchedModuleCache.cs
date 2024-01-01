// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

namespace AE.Projects;

public static class SearchedModuleCache
{
    private static readonly Dictionary<string, SearchedModule> s_Modules = new();

    public static void Clear()
    {
        lock (s_Modules)
        {
            s_Modules.Clear();
        }
    }

    public static bool TryGetModule(string moduleName, [MaybeNullWhen(false)] out SearchedModule module)
    {
        lock (s_Modules)
        {
            return s_Modules.TryGetValue(moduleName, out module);
        }
    }

    public static bool TryAdd(string moduleName, SearchedModule module)
    {
        lock (s_Modules)
        {
            return s_Modules.TryAdd(moduleName, module);
        }
    }
}
