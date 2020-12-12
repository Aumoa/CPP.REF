// Copyright 2020 Aumoa.lib. All right reserved.

using System;
using System.Collections.Generic;
using System.IO;

List<string> FilterFiles(string[] files)
{
    var filtered = new List<string>();
    filtered.Capacity = files.Length;

    foreach (var file in files)
    {
        if (file.EndsWith(".vcxproj"))
        {
            filtered.Add(file);
        }
    }

    return filtered;
}

void RemoveGUID(string file)
{
    string text = File.ReadAllText(file);
    int guidStart = text.IndexOf("<ProjectGuid>");
    if (guidStart == -1)
    {
        return;
    }

    int guidEnd = text.IndexOf("</ProjectGuid>");
    if (guidEnd == -1)
    {
        return;
    }

    string left = text.Substring(0, guidStart);
    string right = text.Substring(guidEnd + "</ProjectGuid>".Length);

    string final = left + right;
    var stream = File.CreateText(file);
    stream.Write(final);
    stream.Flush();
    stream.Close();

    Console.WriteLine($"{file} Cleared.");
}

Console.WriteLine($"Clear project guid in {args[0]} directory.");

var files = Directory.GetFiles(args[0], "*.*", SearchOption.AllDirectories);
var vcprojs = FilterFiles(files);
foreach (var file in vcprojs)
{
    RemoveGUID(file);
}