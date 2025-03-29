using System.Text.Json.Nodes;

namespace AylaEngine;

internal readonly struct SourceCodeCache
{
    private readonly bool m_IsValid;
    private readonly DateTime m_BuildToolWriteTime;
    private readonly DateTime m_SourceCodeWriteTime;
    private readonly DateTime m_RuleFileWriteTime;
    private readonly DateTime[] m_DependsWriteTime;

    private SourceCodeCache(DateTime sourceCodeWriteTime, DateTime buildToolWriteTime, DateTime ruleFileWriteTime, DateTime[] dependsWriteTime)
    {
        m_IsValid = true;
        m_BuildToolWriteTime = buildToolWriteTime;
        m_SourceCodeWriteTime = sourceCodeWriteTime;
        m_RuleFileWriteTime = ruleFileWriteTime;
        m_DependsWriteTime = dependsWriteTime;
    }

    public bool IsModified(in SourceCodeCache other)
    {
        if (m_IsValid == false || other.m_IsValid == false)
        {
            return true;
        }

        return m_SourceCodeWriteTime != other.m_SourceCodeWriteTime
            || m_BuildToolWriteTime != other.m_BuildToolWriteTime
            || m_RuleFileWriteTime != other.m_RuleFileWriteTime
            || m_DependsWriteTime.SequenceEqual(other.m_DependsWriteTime) == false;
    }

    public void SaveCached(string cacheFileName)
    {
        if (File.Exists(cacheFileName))
        {
            File.Delete(cacheFileName);
        }

        using var writer = new BinaryWriter(File.OpenWrite(cacheFileName));
        writer.Write(m_SourceCodeWriteTime.ToBinary());
        writer.Write(m_BuildToolWriteTime.ToBinary());
        writer.Write(m_RuleFileWriteTime.ToBinary());
        writer.Write(m_DependsWriteTime.Length);
        foreach (var w in m_DependsWriteTime)
        {
            writer.Write(w.ToBinary());
        }

        writer.Close();
    }

    public static SourceCodeCache LoadCached(string cacheFileName)
    {
        try
        {
            using var reader = new BinaryReader(File.OpenRead(cacheFileName));
            DateTime sourceCodeWriteTime = DateTime.FromBinary(reader.ReadInt64());
            DateTime buildToolWriteTime = DateTime.FromBinary(reader.ReadInt64());
            DateTime ruleFileWriteTime = DateTime.FromBinary(reader.ReadInt64());
            int dependsCount = reader.ReadInt32();
            DateTime[] dependsWriteTime = new DateTime[dependsCount];
            for (int i = 0; i < dependsCount; i++)
            {
                dependsWriteTime[i] = DateTime.FromBinary(reader.ReadInt64());
            }
            return new SourceCodeCache(sourceCodeWriteTime, buildToolWriteTime, ruleFileWriteTime, dependsWriteTime);
        }
        catch
        {
            return default;
        }
    }

    public static async ValueTask<SourceCodeCache> MakeCachedAsync(string sourceCode, string ruleFilePath, string dependsFileName, CancellationToken cancellationToken)
    {
        if (File.Exists(dependsFileName) == false)
        {
            return default;
        }

        var sourceCodeWriteTime = File.GetLastWriteTimeUtc(sourceCode);
        var buildToolWriteTime = File.GetLastWriteTimeUtc(Global.AssemblyLocation);
        var ruleFileWriteTime = File.GetLastWriteTimeUtc(ruleFilePath);
        string depsJson = await File.ReadAllTextAsync(dependsFileName, cancellationToken);
        var includes = JsonNode.Parse(depsJson)?["Data"]?["Includes"]?.AsArray();
        DateTime[] depsWriteTimes = [];
        if (includes != null)
        {
            depsWriteTimes = new DateTime[includes.Count];
            for (int i = 0; i < includes.Count; i++)
            {
                depsWriteTimes[i] = File.GetLastWriteTimeUtc(includes[i]!.GetValue<string>());
            }
        }
        return new SourceCodeCache(sourceCodeWriteTime, buildToolWriteTime, ruleFileWriteTime, depsWriteTimes);
    }
}
