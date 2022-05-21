// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace DotNETUtilities.Script;

abstract class ScriptWriter : IDisposable
{
    protected readonly ConsoleScriptContext Context;

    public ScriptWriter(ConsoleScriptContext InContext)
    {
        Context = InContext;
    }

    public abstract ScriptWriter StreamIn(int IndexOf);
    public abstract void Dispose();
}
