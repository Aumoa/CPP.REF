// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace DotNETUtilities.Script;

class ConsoleScriptContext
{
    public string? Format;
    public Stack<ConsoleColor> ForegroundColorStack = new();
    public ScriptWriter? CurrentWriter;
}
