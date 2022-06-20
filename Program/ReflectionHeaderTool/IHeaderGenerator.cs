// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Text;

namespace ReflectionHeaderTool;

internal interface IHeaderGenerator
{
    void GenerateHeader(string fileKey, StringBuilder sb);
    void GenerateSource(string fileKey, StringBuilder sb);
}
