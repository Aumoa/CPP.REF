// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Runtime.CompilerServices;

namespace Ayla.SceneManagement;

public static class SceneManager
{
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void LoadScene(Scene scene, LoadSceneMode mode)
        => ScriptingSceneManager.LoadScene(scene, mode);
}
