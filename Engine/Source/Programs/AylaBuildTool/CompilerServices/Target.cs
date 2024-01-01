// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.Rules;
using AE.Source;

namespace AE.CompilerServices;

public static class Target
{
    private static TargetRules? s_Rules;
    private static ProjectDirectory s_ProjectDirectory;

    public static void Clear()
    {
        s_Rules = null;
        s_ProjectDirectory = default;
    }

    public static void Create(TargetRules? rules, ProjectDirectory projectDirectory)
    {
        Debug.Assert(s_Rules == null);
        s_Rules = rules;
        s_ProjectDirectory = projectDirectory;
    }

    public static TargetRules Rules => s_Rules ?? throw new InvalidOperationException("Target is not initialized.");

    public static ProjectDirectory ProjectDirectory => s_ProjectDirectory;
}
