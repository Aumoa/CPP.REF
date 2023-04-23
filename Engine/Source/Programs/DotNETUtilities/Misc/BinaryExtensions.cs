// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

namespace AE.Misc;

public static class BinaryExtensions
{
    public static void Write(this BinaryWriter Writer, IList<string> Elements)
    {
        Writer.Write(Elements.Count);
        foreach (var Elem in Elements)
        {
            Debug.Assert(Elem != null);
            Writer.Write(Elem);
        }
    }

    public static void Write(this BinaryWriter Writer, IList<int> Elements)
    {
        Writer.Write(Elements.Count);
        foreach (var Elem in Elements)
        {
            Writer.Write(Elem);
        }
    }

    public static string[] ReadStringArray(this BinaryReader Reader)
    {
        int Count = Reader.ReadInt32();
        string[] Values = new string[Count];
        for (int N = 0; N < Count; ++N)
        {
            Values[N] = Reader.ReadString();
        }
        return Values;
    }

    public static int[] ReadInt32Array(this BinaryReader Reader)
    {
        int Count = Reader.ReadInt32();
        int[] Values = new int[Count];
        for (int N = 0; N < Count; ++N)
        {
            Values[N] = Reader.ReadInt32();
        }
        return Values;
    }
}
