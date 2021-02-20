// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using System.Collections.Generic;
using System.IO;

List<string> FilterFiles(Dictionary<string, string> path_guid, string[] files)
{
    var filtered = new List<string>();
    filtered.Capacity = files.Length;

    foreach (var file in files)
    {
        if (path_guid.ContainsKey(file))
        {
            filtered.Add(file);
        }
    }

    return filtered;
}

void RemoveGUID(Dictionary<string, string> path_guid, string file)
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

    string left = text.Substring(0, guidStart + "<ProjectGuid>".Length);
    string right = text.Substring(guidEnd);

    string final = left + path_guid[file].ToLower() + right;

    int seekpos = 0;
    while (true)
    {
        int reference_start = final.IndexOf("<ProjectReference ", seekpos);
        if (reference_start == -1)
        {
            break;
        }

        int reference_end = final.IndexOf("</ProjectReference>", reference_start);
        if (reference_end == -1)
        {
            break;
        }

        int include_start = final.IndexOf("Include=\"", reference_start) + "Include=\"".Length;
        int include_end = final.IndexOf('\"', include_start);
        string fullpath = final.Substring(include_start, include_end - include_start);

        foreach (var keys in path_guid)
        {
            if (fullpath.EndsWith(keys.Key))
            {
                int projref_start = final.IndexOf("<Project>", include_end) + "<Project>".Length;
                int projref_end = final.IndexOf("</Project>", projref_start);

                string oldGuid = final.Substring(projref_start, projref_end - projref_start);
                final = final.Replace(oldGuid, keys.Value);
                break;
            }
        }

        seekpos = reference_end;
    }

    var stream = File.CreateText(file);
    stream.Write(final);
    stream.Flush();
    stream.Close();

    Console.WriteLine($"{file} Cleared.");
}

void ParseSingleProject(string projectLine, out string path, out string guid)
{
    string[] left_right = projectLine.Split('=', StringSplitOptions.RemoveEmptyEntries);
    string left = left_right[0];
    string right = left_right[1];

    string[] pars = right.Split(',', StringSplitOptions.RemoveEmptyEntries);
    string name = pars[0];
    path = pars[1].Trim().Trim('\"');
    guid = pars[2].Trim().Trim('\"');

    Console.WriteLine("Dictionary[{0}] = {1}", path, guid);
}

Dictionary<string, string> ParseSolution()
{
    string sln = File.ReadAllText("SC.sln");

    var path_guid = new Dictionary<string, string>();

    int seekpos = 0;
    while (true)
    {
        int beg = sln.IndexOf("Project(\"{", seekpos);
        if (beg == -1)
        {
            break;
        }

        int end = sln.IndexOf("EndProject", beg);
        if (end == -1)
        {
            break;
        }

        ParseSingleProject(sln.Substring(beg, end - beg), out string path, out string guid);
        seekpos = end;

        path_guid.Add(path, guid);
    }

    return path_guid;
}

Console.WriteLine($"Clear {args[0]} projects guid.");

Dictionary<string, string> path_guid = ParseSolution();

var files = Directory.GetFiles(args[0], "*.*", SearchOption.AllDirectories);
var vcprojs = FilterFiles(path_guid, files);
foreach (var file in vcprojs)
{
    RemoveGUID(path_guid, file);
}