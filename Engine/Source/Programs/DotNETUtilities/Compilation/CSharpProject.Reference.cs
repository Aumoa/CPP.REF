// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

public partial class CSharpProject
{
    public record ReferenceBase
    {
        public required string Include { get; init; }
    }

    public record Reference : ReferenceBase
    {
        public string? HintPath { get; init; }

        public override string ToString()
        {
            if (HintPath == null)
            {
                return
                $"""
<Reference Include="{Include}" />
""";
            }
            else
            {
                return
                $"""
<Reference Include="{Include}">
  <HintPath>{HintPath}</HintPath>
</Reference>
""";
            }
        }
    }

    public record ProjectReference : ReferenceBase
    {
        public override string ToString()
        {
            return
$"""
<ProjectReference Include="{Include}" />
""";
        }
    }
}
