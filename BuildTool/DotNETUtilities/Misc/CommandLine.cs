// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System.Collections;
using System.Text;

namespace DotNETUtilites;

public class CommandLine : IEnumerable<string>
{
    public struct CommandArgs
    {
        public string Command;
        public string[] Args;
    }

    Dictionary<string, CommandArgs> KeyValuePairs = new();
    public readonly string[] OriginalArgs;

    public CommandLine(string[] Args)
    {
        StringBuilder SBuilder = null;
        List<string> ResolvedArgs = new();

        foreach (var Arg in Args)
        {
            if (SBuilder == null)
            {
                if (string.IsNullOrEmpty(Arg))
                {
                    continue;
                }

                if (Arg[0] == '\"')
                {
                    if (Arg[^1] == '\"')
                    {
                        ResolvedArgs.Add(Arg[1..^1]);
                    }
                    else
                    {
                        SBuilder = new StringBuilder();
                        SBuilder.Append(Arg[1..^1]);
                    }
                }
                else
                {
                    ResolvedArgs.Add(Arg);
                }
            }
            else
            {
                if (Arg[^1] == '\"')
                {
                    SBuilder.Append(Arg[0..^2]);
                    ResolvedArgs.Add(SBuilder.ToString());
                    ResolvedArgs = null;
                }
                else
                {
                    SBuilder.Append(Arg);
                }
            }
        }

        string CurrentKey = null;
        List<string> CurrentValues = null;

        void FlushKeyValuePair()
        {
            if (CurrentKey != null)
            {
                CommandArgs Args = new()
                {
                    Command = CurrentKey,
                    Args = CurrentValues?.ToArray()
                };

                KeyValuePairs.Add(CurrentKey, Args);
            }

            CurrentKey = null;
            CurrentValues = null;
        }

        for (int i = 0; i < ResolvedArgs.Count; ++i)
        {
            if (ResolvedArgs[i].StartsWith("--"))
            {
                FlushKeyValuePair();
                CurrentKey = ResolvedArgs[i][2..];
            }
            else
            {
                CurrentValues ??= new();
                CurrentValues.Add(ResolvedArgs[i]);
            }
        }

        FlushKeyValuePair();
        OriginalArgs = ResolvedArgs.ToArray();
    }

    public bool TryGetArgument(string Command, out CommandArgs Args)
    {
        return KeyValuePairs.TryGetValue(Command, out Args);
    }

    public IEnumerator<string> GetEnumerator()
    {
        return KeyValuePairs.Keys.GetEnumerator();
    }

    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
}