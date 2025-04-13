using YamlDotNet.Serialization;

namespace AylaEngine;

internal abstract class Project(string name, GroupDescriptor descriptor, Project.Declaration declaration)
{
    public record Declaration
    {
        public Guid Guid { get; init; } = default;

        [YamlIgnore]
        public virtual bool IsValid => Guid != default;

        public static Declaration New() => new Declaration
        {
            Guid = Guid.NewGuid()
        };
    }

    public readonly string Name = name;
    public readonly GroupDescriptor Group = descriptor;
    public readonly Declaration Decl = declaration;

    public bool IsEngine => Group.IsEngine;

    public override string ToString()
    {
        return $"({GetType().Name[..^("Project".Length)]}) {Name} {Decl.Guid.ToString("P").ToUpper()}";
    }
}
