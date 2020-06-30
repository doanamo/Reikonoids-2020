using UnrealBuildTool;

public class Reikonoids : ModuleRules
{
    public Reikonoids(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "GameplayTasks", "UMG", "Slate", "SlateCore" });
    }
}
