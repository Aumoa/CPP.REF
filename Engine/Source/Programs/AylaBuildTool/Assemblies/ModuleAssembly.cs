// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Reflection;
using AE.CompilerServices;
using AE.Exceptions;
using AE.Rules;

namespace AE.Assemblies;

public abstract class ModuleAssembly : ScriptableAssembly
{
    private static readonly Dictionary<TargetInfo, ModuleRules> s_RulesCache = new();

    protected override bool HasAssembly => true;

    protected virtual string ClassName => Name;

    public ModuleRules Rules => GenerateModuleRule();

    private ModuleRules GenerateModuleRule()
    {
        Debug.Assert(CachedAssembly != null, "Need configure first.");

        lock (s_RulesCache)
        {
            if (s_RulesCache.TryGetValue(Target.Info, out var moduleRule) == false)
            {
                Type? ruleType = CachedAssembly.GetType(ClassName);
                if (ruleType == null)
                {
                    throw new TerminateException(KnownErrorCode.ConstructorNotFound, CoreStrings.Errors.RuleClassDefinitions(ClassName));
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
