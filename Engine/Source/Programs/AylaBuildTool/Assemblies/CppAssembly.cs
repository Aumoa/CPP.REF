// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;

using AE.CompilerServices;
using AE.Exceptions;
using AE.IO;
using AE.Rules;
using AE.Source;

namespace AE.Assemblies;

public class CppAssembly : ModuleAssembly
{
    private readonly Dictionary<TargetInfo, ModuleRules> rulesCache = new();

    [SetsRequiredMembers]
    public CppAssembly(ProjectDirectory projectDirectory, FileReference scriptFile)
    {
        Name = scriptFile.Name;
        ProjectDirectory = projectDirectory;
        SourceDirectory = scriptFile.GetDirectory();
        ScriptFile = scriptFile;
    }

    public override string Name { get; }

    public override ProjectDirectory ProjectDirectory { get; }

    public override DirectoryReference SourceDirectory { get; }

    public override FileReference ScriptFile { get; }

    protected override bool HasAssembly => true;

    public ModuleRules Rules => GenerateModuleRule();

    private ModuleRules GenerateModuleRule()
    {
        Debug.Assert(CachedAssembly != null, "Need configure first.");

        lock (rulesCache)
        {
            if (rulesCache.TryGetValue(Target.Info, out var moduleRule) == false)
            {
                Type? ruleType = CachedAssembly.GetType(Name);
                if (ruleType == null)
                {
                    throw new TerminateException(KnownErrorCode.ConstructorNotFound, CoreStrings.Errors.RuleClassDefinitions(Name));
                }

                ConstructorInfo? constructor = ruleType.GetConstructor(new[] { typeof(TargetInfo) });
                if (constructor == null)
                {
                    throw new TerminateException(KnownErrorCode.ConstructorNotFound, CoreStrings.Errors.ModuleRuleConstructorNotFound(ruleType.Name));
                }

                moduleRule = (ModuleRules)constructor.Invoke(new object?[] { Target.Info });
            }

            return moduleRule;
        }
    }
}
