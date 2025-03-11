// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode.h"

#include "Component/RockDelegateConnectorComponent.h"
#include "Components/BillboardComponent.h"


ARockGameplayNode::ARockGameplayNode()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetMobility(EComponentMobility::Static);
	
#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	SpriteComponent->SetupAttachment(RootComponent);
	if (!IsRunningCommandlet())
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;
			FName ID_Effects;
			FText NAME_Effects;
			FConstructorStatics()
				: SpriteTextureObject(TEXT("/Niagara/Icons/S_ParticleSystem"))
				, ID_Effects(TEXT("Effects"))
				, NAME_Effects(NSLOCTEXT("SpriteCategory", "Effects", "Effects"))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		if (SpriteComponent)
		{
			SpriteComponent->Sprite = ConstructorStatics.SpriteTextureObject.Get();
			// 100 above 'root' location. So it's not in the ground.
			// Later on perhaps we put a 'rock' that sticks to the ground, and the symbol up top. Like how UEFN does it?
			SpriteComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
			SpriteComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
			SpriteComponent->bHiddenInGame = true;
			SpriteComponent->bIsScreenSizeScaled = true;
			SpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_Effects;
			SpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Effects;
			SpriteComponent->SetupAttachment(RootComponent);
			SpriteComponent->bReceivesDecals = false;

			SpriteComponent->SetUsingAbsoluteScale(true);
		}
	}
#endif

	
	GameplayConnector = CreateDefaultSubobject<URockDelegateConnectorComponent>(TEXT("DelegateConnector"));
	// GameplayConnector->OnComponentCreated();
	// GameplayConnector->RegisterComponent();
}

void ARockGameplayNode::Enable(AActor* EventInstigator)
{
	bIsEnabled = true;
}

void ARockGameplayNode::Disable(AActor* EventInstigator)
{
	bIsEnabled = false;
}

bool ARockGameplayNode::IsEnabled() const
{
	return bIsEnabled;
}

void ARockGameplayNode::TriggerOutput()
{
	if (bIsEnabled)
	{
		OnTriggered.Broadcast();
	}
}
