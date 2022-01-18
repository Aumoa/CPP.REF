// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using DotNETUtilities;

namespace BuildTool;

class Project : CompiledModuleCode<ModuleRule>
{
    public readonly DirectoryReference ProjectDir;

    public Project(FileReference ProjectRuleCode) : base(ProjectRuleCode)
    {
        ProjectDir = RuleCode.GetParent();
    }
}