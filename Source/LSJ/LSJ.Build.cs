using UnrealBuildTool;

public class LSJ : ModuleRules
{
    public LSJ(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" , "Tekken8", "UMG", "Kyoulee", "JMH","Niagara"});
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

    }
}