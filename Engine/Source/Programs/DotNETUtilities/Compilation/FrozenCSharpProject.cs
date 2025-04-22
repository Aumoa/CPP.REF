// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

public record FrozenCSharpProject
{
    public required CSharpProject.PropertyGroup PropertyGroup { get; init; }

    public required CSharpProject.ItemGroup ItemGroup { get; init; }
}
