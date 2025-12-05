using System.IO;
using UnrealBuildTool;

public class CR : ModuleRules
{
    public CR(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "Slate",
            "SlateCore",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem"
        });

        // ============ 添加这行！告诉编译器在哪里找头文件 ============
        PublicIncludePaths.Add(ModuleDirectory);

        // 添加更多包含路径（可选）
        PublicIncludePaths.AddRange(new string[]
        {
            Path.Combine(ModuleDirectory, "Public/Game"),
            Path.Combine(ModuleDirectory, "Public/Actors"),
            Path.Combine(ModuleDirectory, "Public/Actors/Units"),
            Path.Combine(ModuleDirectory, "Public/Actors/Buildings"),
            Path.Combine(ModuleDirectory, "Public/Components"),
            Path.Combine(ModuleDirectory, "Public/Components/Combat"),
            Path.Combine(ModuleDirectory, "Public/Data"),
            Path.Combine(ModuleDirectory, "Public/Systems"),
            Path.Combine(ModuleDirectory, "Public/UI"),
            Path.Combine(ModuleDirectory, "Public/AI")
        });

        // 私有包含路径
        PrivateIncludePaths.AddRange(new string[]
        {
            Path.Combine(ModuleDirectory, "Private/Game"),
            Path.Combine(ModuleDirectory, "Private/Actors"),
            Path.Combine(ModuleDirectory, "Private/Components"),
            Path.Combine(ModuleDirectory, "Private/Systems"),
            Path.Combine(ModuleDirectory, "Private/UI"),
            Path.Combine(ModuleDirectory, "Private/AI")
        });


    }
}