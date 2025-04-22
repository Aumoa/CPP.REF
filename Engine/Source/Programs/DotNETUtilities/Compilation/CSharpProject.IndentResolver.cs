// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

public partial class CSharpProject
{
    private class IndentResolver
    {
        public IndentResolver()
        {
        }

        public string Indent(Func<IndentResolver, string> func)
        {
            return string.Join('\n', func(this).Split('\n').Select(p => "  " + p));
        }
    }
}
