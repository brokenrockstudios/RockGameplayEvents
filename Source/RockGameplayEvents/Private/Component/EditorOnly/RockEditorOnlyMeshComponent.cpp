// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Component/EditorOnly/RockEditorOnlyMeshComponent.h"

URockEditorOnlyMeshComponent::URockEditorOnlyMeshComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

#if WITH_EDITORONLY_DATA
	// struct FConstructorStatics
	// {
	// 	ConstructorHelpers::FObjectFinderOptional<UTexture2D> TriggerTextureObject;
	// 	FName ID_Triggers;
	// 	FText NAME_Triggers;
	// 	FConstructorStatics()
	// 		: TriggerTextureObject(TEXT("/Engine/EditorResources/S_TriggerBox"))
	// 			, ID_Triggers(TEXT("Triggers"))
	// 			, NAME_Triggers(NSLOCTEXT("SpriteCategory", "Triggers", "Triggers"))
	// 	{
	// 	}
	// };
	// static FConstructorStatics ConstructorStatics;

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinder<UTexture2D> SpriteTexture;
		FName ID_Misc;
		FText NAME_Misc;
		FConstructorStatics()
			: SpriteTexture(TEXT("/Engine/EditorResources/S_TriggerBox"))
			, ID_Misc(TEXT("Misc"))
			, NAME_Misc(NSLOCTEXT( "SpriteCategory", "Misc", "Misc" ))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	//Sprite = ConstructorStatics.SpriteTexture.Object;
#endif

	bUseEditorCompositing = true;
	bHiddenInGame = true;
	SetGenerateOverlapEvents(false);
	
	
}
