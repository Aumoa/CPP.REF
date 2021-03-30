// Copyright 2020-2021 Aumoa.lib. All right reserved.

public class AssimpBuild : BuildRules
{
    public AssimpBuild()
    {
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Zlib"
        });

        AdditionalPreprocessorDefines.AddRange(new string[]
        {
            "_CRT_SECURE_NO_WARNINGS",
            "ASSIMP_BUILD_NO_C4D_IMPORTER",
            "ASSIMP_BUILD_NO_IFC_IMPORTER",
            "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING",
            "OPENDDL_STATIC_LIBARY",
            "ASSIMP_BUILD",
            "RAPIDJSON_HAS_STDSTRING",
            "MINIZ_USE_UNALIGNED_LOADS_AND_STORES=0",
            "RAPIDJSON_NOMEMBERITERATORCLASS",
            "OPENDDLPARSER_BUILD",
        });

        CppStandardVersion = CppVersion.Cpp14;
    }
}