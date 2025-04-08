using System.Text.Json.Nodes;

namespace AylaEngine;

internal readonly struct SourceCodeCache
{
    private readonly bool m_IsValid;
    private readonly DateTime m_BuildToolWriteTime;
    private readonly DateTime m_SourceCodeWriteTime;
    private readonly DateTime m_RuleFileWriteTime;
    private readonly DateTime[] m_DependRuleFilesWriteTime;
    private readonly DateTime[] m_DependsWriteTime;

    private SourceCodeCache(DateTime sourceCodeWriteTime, DateTime buildToolWriteTime, DateTime ruleFileWriteTime, DateTime[] dependRuleFilesWriteTime, DateTime[] dependsWriteTime)
    {
        m_IsValid = true;
        m_BuildToolWriteTime = buildToolWriteTime;
        m_SourceCodeWriteTime = sourceCodeWriteTime;
        m_RuleFileWriteTime = ruleFileWriteTime;
        m_DependRuleFilesWriteTime = dependRuleFilesWriteTime;
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
            || m_DependRuleFilesWriteTime.SequenceEqual(other.m_DependRuleFilesWriteTime) == false
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
        Write(writer, m_DependRuleFilesWriteTime);
        Write(writer, m_DependsWriteTime);
        writer.Close();

        return;

        void Write(BinaryWriter writer, ReadOnlySpan<DateTime> values)
        {
            writer.Write(values.Length);
            foreach (var v in values)
            {
                writer.Write(v.ToBinary());
            }
        }
    }

    public static SourceCodeCache LoadCached(string cacheFileName)
    {
        try
        {
            using var reader = new BinaryReader(File.OpenRead(cacheFileName));
            DateTime sourceCodeWriteTime = DateTime.FromBinary(reader.ReadInt64());
            DateTime buildToolWriteTime = DateTime.FromBinary(reader.ReadInt64());
            DateTime ruleFileWriteTime = DateTime.FromBinary(reader.ReadInt64());
            DateTime[] dependRuleFilesWriteTime = ReadDateTimeArray(reader);
            DateTime[] dependsWriteTime = ReadDateTimeArray(reader);
            reader.Close();
            return new SourceCodeCache(sourceCodeWriteTime, buildToolWriteTime, ruleFileWriteTime, dependRuleFilesWriteTime, dependsWriteTime);

            DateTime[] ReadDateTimeArray(BinaryReader reader)
            {
                int count = reader.ReadInt32();
                var result = new DateTime[count];
                for (int i = 0; i < count; ++i)
                {
                    result[i] = DateTime.FromBinary(reader.ReadInt64());
                }

                return result;
            }
        }
        catch
        {
            return default;
        }
    }

    public static async ValueTask<SourceCodeCache> MakeCachedAsync(string sourceCode, string ruleFilePath, string dependsFileName, string[] dependRuleFileNames, CancellationToken cancellationToken)
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
        DateTime[] dependRuleFilesWriteTime = new DateTime[dependRuleFileNames.Length];
        for (int i = 0; i < dependRuleFileNames.Length; ++i)
        {
            dependRuleFilesWriteTime[i] = File.GetLastWriteTimeUtc(dependRuleFileNames[i]);
        }
        return new SourceCodeCache(sourceCodeWriteTime, buildToolWriteTime, ruleFileWriteTime, dependRuleFilesWriteTime, depsWriteTimes);
    }

    public static SourceCodeCache MakeCachedSimple(string sourceCode, string ruleFilePath)
    {
        var sourceCodeWriteTime = File.GetLastWriteTimeUtc(sourceCode);
        var buildToolWriteTime = File.GetLastWriteTimeUtc(Global.AssemblyLocation);
        var ruleFileWriteTime = File.GetLastWriteTimeUtc(ruleFilePath);
        return new SourceCodeCache(sourceCodeWriteTime, buildToolWriteTime, ruleFileWriteTime, Array.Empty<DateTime>(), Array.Empty<DateTime>());
    }
}
