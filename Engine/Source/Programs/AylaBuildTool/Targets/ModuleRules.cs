using System.Collections.Frozen;
using System.Diagnostics;

namespace AylaEngine;

public class ModuleRules
{
    private static class ConstructorArgs
    {
        public static ThreadLocal<ITargetInfo> s_TargetInfo = new();
    }

    public readonly ITargetInfo TargetInfo = ConstructorArgs.s_TargetInfo.Value
        ?? throw new InvalidOperationException("ModuleRules must be created using ModuleRules.New function.");

    private bool m_FreezeCollections;

    public IReadOnlySet<string> PublicIncludePaths { get; private set; } = new HashSet<string>();
    public IReadOnlySet<string> PrivateIncludePaths { get; private set; } = new HashSet<string>();

    public IReadOnlySet<int> PublicDisableWarnings { get; private set; } = new HashSet<int>();
    public IReadOnlySet<int> PrivateDisableWarnings { get; private set; } = new HashSet<int>();

    public IReadOnlySet<string> PublicAdditionalMacros { get; private set; } = new HashSet<string>();
    public IReadOnlySet<string> PrivateAdditionalMacros { get; private set; } = new HashSet<string>();

    public IReadOnlySet<string> PublicDependencyModuleNames { get; private set; } = new HashSet<string>();
    public IReadOnlySet<string> PrivateDependencyModuleNames { get; private set; } = new HashSet<string>();

    public IReadOnlySet<string> PublicAdditionalLibraries { get; private set; } = new HashSet<string>();
    public IReadOnlySet<string> PrivateAdditionalLibraries { get; private set; } = new HashSet<string>();

    protected ModuleRules()
    {
    }

    public ModuleType Type { get; protected init; } = ModuleType.Library;

    public string Name => GetType().Name;

    public void AddPublicIncludePaths(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PublicIncludePaths).AddRange(items);
    }

    public void AddPrivateIncludePaths(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PrivateIncludePaths).AddRange(items);
    }

    public void AddPublicDisableWarnings(params int[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<int>)PublicDisableWarnings).AddRange(items);
    }

    public void AddPrivateDisableWarnings(params int[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<int>)PrivateDisableWarnings).AddRange(items);
    }

    public void AddPublicAdditionalMacros(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PublicAdditionalMacros).AddRange(items);
    }

    public void AddPrivateAdditionalMacros(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PrivateAdditionalMacros).AddRange(items);
    }

    public void AddPublicDependencyModuleNames(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PublicDependencyModuleNames).AddRange(items);
    }

    public void AddPrivateDependencyModuleNames(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PrivateDependencyModuleNames).AddRange(items);
    }

    public void AddPublicAdditionalLibraries(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PublicAdditionalLibraries).AddRange(items);
    }

    public void AddPrivateAdditionalLibraries(params string[] items)
    {
        if (m_FreezeCollections)
        {
            throw new InvalidOperationException();
        }

        ((HashSet<string>)PrivateAdditionalLibraries).AddRange(items);
    }

    internal void FreezeCollections()
    {
        m_FreezeCollections = true;
        PublicIncludePaths = ((HashSet<string>)PublicIncludePaths).ToFrozenSet();
        PrivateIncludePaths = ((HashSet<string>)PrivateIncludePaths).ToFrozenSet();
        PublicDisableWarnings = ((HashSet<int>)PublicDisableWarnings).ToFrozenSet();
        PrivateDisableWarnings = ((HashSet<int>)PrivateDisableWarnings).ToFrozenSet();
        PublicAdditionalMacros = ((HashSet<string>)PublicAdditionalMacros).ToFrozenSet();
        PrivateAdditionalMacros = ((HashSet<string>)PrivateAdditionalMacros).ToFrozenSet();
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
}
