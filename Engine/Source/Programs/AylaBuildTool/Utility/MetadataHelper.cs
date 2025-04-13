using YamlDotNet.Core;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace AylaEngine;

internal static class MetadataHelper
{
    private static readonly ISerializer s_Serializer = new SerializerBuilder()
        .Build();

    private static readonly IDeserializer s_Deserializer = new DeserializerBuilder()
        .Build();

    public static string Serialize<T>(T instance)
    {
        return s_Serializer.Serialize(instance);
    }

    public static async Task SerializeToFileAsync<T>(T instance, string filePath, CancellationToken cancellationToken = default)
    {
        Dictionary<string, T> remap = [];
        remap.Add(typeof(T).Name, instance);
        var yaml = Serialize(remap);
        await File.WriteAllTextAsync(filePath, yaml, cancellationToken);
    }

    public static T? Deserialize<T>(string yaml)
    {
        try
        {
            var remap = s_Deserializer.Deserialize<Dictionary<string, T>>(yaml);
            return remap[typeof(T).Name];
        }
        catch (YamlException)
        {
            return default;
        }
    }

    public static async Task<T?> DeserializeFromFileAsync<T>(string filePath, CancellationToken cancellationToken = default)
    {
        try
        {
            var yaml = await File.ReadAllTextAsync(filePath, cancellationToken);
            return Deserialize<T>(yaml);
        }
        catch (FileNotFoundException)
        {
            return default;
        }
    }
}
