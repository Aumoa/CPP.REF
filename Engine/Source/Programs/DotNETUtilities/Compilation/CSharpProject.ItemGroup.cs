// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

public partial class CSharpProject
{

    public record ItemGroup
    {
        public Condition? Condition { get; init; }
        
        public IEnumerable<ReferenceBase>? References { get; init; }

        public bool IsEmpty => References == null || References.Any() == false;
    }

    private string GenerateXml(ItemGroup value, IndentResolver indent)
    {
        string? conditionString = null;
        if (value.Condition != null)
        {
            conditionString = $" Condition=\"{value.Condition.ToString()}\"";
        }

        return
$"""
<ItemGroup{conditionString}>
  {GenerateInnerXml(indent)}
</ItemGroup>
""";

        string GenerateInnerXml(IndentResolver indent)
        {
            return string.Join('\n', [..(value.References ?? Array.Empty<ReferenceBase>())]);
        }
    }
}
