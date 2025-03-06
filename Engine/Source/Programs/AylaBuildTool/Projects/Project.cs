using YamlDotNet.Serialization;

namespace AylaEngine;

internal abstract class Project(string name, GroupDescriptor descriptor, Project.Declaration declaration)
{
    public record Declaration
    {
        public Guid Guid { get; init; } = default;

        [YamlIgnore]
        public bool IsValid => Guid != default;

        public static Declaration New() => new Declaration { Guid = Guid.NewGuid() };
    }

    public readonly string Name = name;
    public readonly GroupDescriptor Descriptor = descriptor;
    public readonly Declaration Decl = declaration;

    public override string ToString()
    {
        return $"({GetType().Name[..^("Project".Length)]}) {Name} {Decl.Guid.ToString("P").ToUpper()}";
    }
}
