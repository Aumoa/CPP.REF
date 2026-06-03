using System.Collections.Frozen;
using System.Diagnostics;

namespace AylaEngine;

public class ModuleRules
{
    private static class ConstructorArgs
    {
        public static ThreadLocal<ITargetInfo> s_TargetInfo = new();
    }

    public struct ThirdPartyConfiguration
    {
        public string CMakeSource { get; set; }
    }

    public readonly ITargetInfo TargetInfo = ConstructorArgs.s_TargetInfo.Value
        ?? throw new InvalidOperationException("ModuleRules must be created using ModuleRules.New function.");

    private bool m_FreezeCollections;
    private readonly List<string> m_Errors = [];

    public IReadOnlySet<string> PublicIncludePaths { get; private set; } = new HashSet<string>();
    public IReadOnlySet<string> PrivateIncludePaths { get; private set; } = new HashSet<string>();

    public IReadOnlySet<int> PublicDisableWarnings { get; private set; } = new HashSet<int>();
    public IReadOnlySet<int> PrivateDisableWarnings { get; private set; } = new HashSet<int>();

    public IReadOnlySet<MacroSet> PublicAdditionalMacros { get; private set; } = new HashSet<MacroSet>();
    public IReadOnlySet<MacroSet> PrivateAdditionalMacros { get; private set; } = new HashSet<MacroSet>();

    public IReadOnlySet<string> PublicDependencyModuleNames { get; private set; } = new HashSet<string>();
    public IReadOnlySet<string> PrivateDependencyModuleNames { get; private set; } = new HashSet<string>();

    public IReadOnlySet<string> PublicAdditionalLibraries { get; private set; } = new HashSet<string>();
    public IReadOnlySet<string> PrivateAdditionalLibraries { get; private set; } = new HashSet<string>();

    public ThirdPartyConfiguration ThirdParty
    {
        get;
        init;
    } = new()
    {
        CMakeSource = "src"
    };

    protected ModuleRules()
    {
    }

    public ModuleType Type { get; protected init; } = ModuleType.Library;

    public string Name => GetType().Name.Replace('_', '.');

    public string SafeName => Name.Replace('.', '_');

    public ScriptRule Script { get; } = new();

    protected void AddError(string error)
    {
        m_Errors.Add(error);
    }

    public void ThrowErrors()
    {
        if (m_Errors.Count > 0)
        {
            throw new InvalidOperationException(string.Join('\n', m_Errors));
        }
    }

    public void AddPublicIncludePaths(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PublicIncludePaths).UnionWith(items);
    }

    public void AddPrivateIncludePaths(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PrivateIncludePaths).UnionWith(items);
    }

    public void AddPublicDisableWarnings(params int[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<int>)PublicDisableWarnings).UnionWith(items);
    }

    public void AddPrivateDisableWarnings(params int[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<int>)PrivateDisableWarnings).UnionWith(items);
    }

    public void AddPublicAdditionalMacros(params MacroSet[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<MacroSet>)PublicAdditionalMacros).UnionWith(items);
    }

    public void AddPrivateAdditionalMacros(params MacroSet[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<MacroSet>)PrivateAdditionalMacros).UnionWith(items);
    }

    public void AddPublicDependencyModuleNames(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PublicDependencyModuleNames).UnionWith(items);
    }

    public void AddPrivateDependencyModuleNames(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PrivateDependencyModuleNames).UnionWith(items);
    }

    public void AddPublicAdditionalLibraries(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PublicAdditionalLibraries).UnionWith(items);
    }

    public void AddPrivateAdditionalLibraries(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PrivateAdditionalLibraries).UnionWith(items);
    }

    internal void FreezeCollections()
    {
        m_FreezeCollections = true;
        PublicIncludePaths = ((HashSet<string>)PublicIncludePaths).ToFrozenSet();
        PrivateIncludePaths = ((HashSet<string>)PrivateIncludePaths).ToFrozenSet();
        PublicDisableWarnings = ((HashSet<int>)PublicDisableWarnings).ToFrozenSet();
        PrivateDisableWarnings = ((HashSet<int>)PrivateDisableWarnings).ToFrozenSet();
        PublicAdditionalMacros = ((HashSet<MacroSet>)PublicAdditionalMacros).ToFrozenSet();
        PrivateAdditionalMacros = ((HashSet<MacroSet>)PrivateAdditionalMacros).ToFrozenSet();
        PublicDependencyModuleNames = ((HashSet<string>)PublicDependencyModuleNames).ToFrozenSet();
        PrivateDependencyModuleNames = ((HashSet<string>)PrivateDependencyModuleNames).ToFrozenSet();
        PublicAdditionalLibraries = ((HashSet<string>)PublicAdditionalLibraries).ToFrozenSet();
        PrivateAdditionalLibraries = ((HashSet<string>)PrivateAdditionalLibraries).ToFrozenSet();
    }

    public static ModuleRules New(Type ruleType, ITargetInfo targetInfo)
    {
        try
        {
            Debug.Assert(ConstructorArgs.s_TargetInfo.Value == null);
            ConstructorArgs.s_TargetInfo.Value = targetInfo;
            return (ModuleRules)Activator.CreateInstance(ruleType)!;
        }
        finally
        {
            ConstructorArgs.s_TargetInfo.Value = null!;
        }
    }

    public bool IsSharedLibrary() => Type is ModuleType.Library or ModuleType.Game;
}
