// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Misc;

using System.Diagnostics;
using System.Text;

namespace AE.Rules;

public class TargetRules
{
    public TargetRules(TargetInfo Info)
    {
        Name = GetType().Name.Replace("Target", "");
        Debug.Assert(Info.Name != null);
        TargetName = Info.Name;
        Class = TargetClass.Game;
        Platform = Info.BuildConfiguration.Platform;
        TargetModuleName = Name;
    }

    private TargetRules()
    {
        Name = null!;
        TargetName = null!;
        Platform = null!;
        TargetModuleName = null!;
    }

    public string Name { get; private set; }

    public string TargetName { get; protected set; }

    public string SourceCode { get; set; } = null!;

    public TargetClass Class { get; protected set; }

    public TargetType Type { get; protected set; }

    public TargetPlatform Platform { get; protected set; }

    public string TargetModuleName { get; protected set; }

    public bool bBuildAllModules { get; set; }

    public void Serialize(BinaryWriter Writer)
    {
        Writer.Write(Name);
        Writer.Write(TargetName);
        Writer.Write(SourceCode);
        Writer.Write((int)Class);
        Writer.Write((int)Type);
        Platform.Serialize(Writer);
        Writer.Write(TargetModuleName);
        Writer.Write(bBuildAllModules);
    }

    public static TargetRules Deserialize(BinaryReader Reader)
    {
        TargetRules Rules = new();
        Rules.Name = Reader.ReadString();
        Rules.TargetName = Reader.ReadString();
        Rules.SourceCode = Reader.ReadString();
        Rules.Class = (TargetClass)Reader.ReadInt32();
        Rules.Type = (TargetType)Reader.ReadInt32();
        Rules.Platform = TargetPlatform.Deserialize(Reader);
        Rules.TargetModuleName = Reader.ReadString();
        Rules.bBuildAllModules = Reader.ReadBoolean();
        return Rules;
    }
}
