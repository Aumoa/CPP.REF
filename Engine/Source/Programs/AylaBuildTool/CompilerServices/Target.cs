// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public static class Target
{
    public struct State
    {
        public TargetInfo? targetInfo;
        public bool isEditor;
    }

    private static readonly Stack<State> s_Stack = new();
    private static State s_Current;

    public static void Clear()
    {
        s_Current = default;
    }

    public static void Push()
    {
        s_Stack.Push(s_Current);
    }

    public static void Pop()
    {
        s_Current = s_Stack.Pop();
    }

    public static void Create(TargetInfo targetInfo, bool isEditor)
    {
        s_Current.targetInfo = targetInfo;
        s_Current.isEditor = isEditor;
    }

    public static TargetInfo Info => s_Current.targetInfo ?? throw new InvalidOperationException("Target is not initialized.");

    public static BuildConfiguration Config => Info.BuildConfiguration;

    public static Architecture Architecture => Config.Platform.Architecture;

    public static Configuration Configuration => Config.Configuration;

    public static TargetPlatform Platform => Config.Platform;

    public static bool IsEditor => s_Current.isEditor;
}
