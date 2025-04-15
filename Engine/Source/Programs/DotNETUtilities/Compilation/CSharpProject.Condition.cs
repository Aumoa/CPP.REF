// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

public partial class CSharpProject
{
    public record Condition
    {
        public required string? Configuration { get; init; }

        public required string? Platform { get; init; }

        public override string ToString()
        {
            List<string> left = [];
            List<string> right = [];

            if (Configuration != null)
            {
                left.Add("Configuration");
                right.Add(Configuration);
            }

            if (Platform != null)
            {
                left.Add("Platform");
                right.Add(Platform);
            }

            return $"'{string.Join('|', left.Select(p => $"$({p})"))}'=='{string.Join('|', right)}'";
        }
    }
}
