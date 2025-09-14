namespace AylaEngine;

internal record struct NamespaceName(params string[] Names) : IScopeName
{
    public string Cpp(string name)
    {
        if (Names.Length == 0)
        {
            return "::" + name;
        }
        else
        {
            return "::" + string.Join("::", Names) + "::" + name;
        }
    }
    
    public string CSharp(string name)
    {
        if (Names.Length == 0)
        {
            return "global::" + name;
        }
        else
        {
            return "global::" + string.Join(".", Names) + "." + name;
        }
    }

    public static readonly NamespaceName Std = new NamespaceName("std");
    public static readonly NamespaceName Ayla = new NamespaceName("Ayla");
}
