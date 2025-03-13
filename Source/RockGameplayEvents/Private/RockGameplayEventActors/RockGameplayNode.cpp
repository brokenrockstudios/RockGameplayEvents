// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode.h"

#include "Component/RockDelegateConnectorComponent.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"


ARockGameplayNode::ARockGameplayNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	// set collision ignore all
	// MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetMobility(EComponentMobility::Movable);

	RootComponent = MeshComponent;

	// #if true

	// if (!IsRunningCommandlet() && !IsRunningDedicatedServer())
	{
		// THIS IS ALL UGLY
		// Can we spin this off to like a custom editor component or something?
		EditorOnly_Sprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
		if (EditorOnly_Sprite)
		{
			// Structure to hold one-time initialization
			struct FConstructorStatics
			{
				ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;
				FName ID_Effects;
				FText NAME_Effects;

				FConstructorStatics()
					: SpriteTextureObject(TEXT("/Engine/EditorResources/Bad"))
					  , ID_Effects(TEXT("Effects"))
					  , NAME_Effects(NSLOCTEXT("SpriteCategory", "Effects", "Effects1"))
				{
				}
			};
			static FConstructorStatics ConstructorStatics;
			EditorOnly_Sprite->SetMobility(EComponentMobility::Movable);
			EditorOnly_Sprite->Sprite = ConstructorStatics.SpriteTextureObject.Get();
			EditorOnly_Sprite->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			EditorOnly_Sprite->SetRelativeScale3D(FVector(1, 1, 1));
			EditorOnly_Sprite->bHiddenInGame = true;
			EditorOnly_Sprite->bIsScreenSizeScaled = true;
			EditorOnly_Sprite->SpriteInfo.Category = ConstructorStatics.ID_Effects;
			EditorOnly_Sprite->SpriteInfo.DisplayName = ConstructorStatics.NAME_Effects;
			EditorOnly_Sprite->SetupAttachment(MeshComponent);
			EditorOnly_Sprite->bReceivesDecals = false;
			EditorOnly_Sprite->SetUsingAbsoluteScale(true);
		}
		EditorOnly_RockBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RockBase"));

		if (EditorOnly_RockBase)
		{
			struct FConstructorStatics
			{
				ConstructorHelpers::FObjectFinderOptional<UStaticMesh> SpriteTextureObject;
				FName ID_Effects;
				FText NAME_Effects;

				FConstructorStatics()
					: SpriteTextureObject(TEXT("/RockGameplayEvents/Rock_Base"))
					  , ID_Effects(TEXT("Effects"))
					  , NAME_Effects(NSLOCTEXT("SpriteCategory", "Effects", "Effects2"))
				{
				}
			};
			static FConstructorStatics ConstructorStatics;

			EditorOnly_RockBase->SetMobility(EComponentMobility::Movable);
			EditorOnly_RockBase->SetStaticMesh(ConstructorStatics.SpriteTextureObject.Get());
			// 100 above 'root' location. So it's not in the ground.
			// Later on perhaps we put a 'rock' that sticks to the ground, and the symbol up top. Like how UEFN does it?
			// Line trace down from the Sprite to the ground
			FVector RockBaseLocation = EditorOnly_RockBase->GetRelativeLocation();
			EditorOnly_RockBase->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			EditorOnly_RockBase->SetRelativeScale3D(FVector(.75f, 0.75f, 0.75f));
			EditorOnly_RockBase->bHiddenInGame = true;
			//EditorOnly_RockBase->bIsScreenSizeScaled = true;
			//EditorOnly_RockBase->SpriteInfo.Category = ConstructorStatics.ID_Effects;
			//EditorOnly_RockBase->SpriteInfo.DisplayName = ConstructorStatics.NAME_Effects;
			EditorOnly_RockBase->SetupAttachment(EditorOnly_Sprite);
			//EditorOnly_RockBase->AttachToComponent(EditorOnly_Sprite, FAttachmentTransformRules::KeepRelativeTransform);
			//EditorOnly_RockBase->Keep
			EditorOnly_RockBase->bReceivesDecals = false;

			EditorOnly_RockBase->SetUsingAbsoluteScale(true);
		}
		EditorOnly_Beam = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beam"));
		if (EditorOnly_Beam)
		{
			struct FConstructorStatics
			{
				ConstructorHelpers::FObjectFinderOptional<UStaticMesh> SpriteTextureObject;
				FName ID_Effects;
				FText NAME_Effects;

				FConstructorStatics()
					: SpriteTextureObject(TEXT("/RockGameplayEvents/Rock_Beam"))
					  , ID_Effects(TEXT("Effects"))
					  , NAME_Effects(NSLOCTEXT("SpriteCategory", "Effects", "Effects3"))
				{
				}
			};
			EditorOnly_Beam->SetMobility(EComponentMobility::Movable);
			static FConstructorStatics ConstructorStatics;
			EditorOnly_Beam->SetStaticMesh(ConstructorStatics.SpriteTextureObject.Get());
			// 100 above 'root' location. So it's not in the ground.
			// Later on perhaps we put a 'rock' that sticks to the ground, and the symbol up top. Like how UEFN does it?
			// Calculate height difference distance between Base and Sprite dynamically.
			EditorOnly_Beam->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			float HeightDifference = EditorOnly_Beam->GetRelativeLocation().Z - EditorOnly_Sprite->GetRelativeLocation().Z;
			HeightDifference = FMath::Clamp(HeightDifference, 1, 300);
			EditorOnly_Beam->SetRelativeScale3D(FVector(1, 1, 1));
			EditorOnly_Beam->bHiddenInGame = true;
			//EditorOnly_Beam->bIsScreenSizeScaled = true;
			//EditorOnly_Beam->SpriteInfo.Category = ConstructorStatics.ID_Effects;
			//EditorOnly_Beam->SpriteInfo.DisplayName = ConstructorStatics.NAME_Effects;
			EditorOnly_Beam->SetupAttachment(EditorOnly_RockBase);
			EditorOnly_Beam->bReceivesDecals = false;
			EditorOnly_Beam->SetUsingAbsoluteScale(true);
		}
	}
	// #endif

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

void ARockGameplayNode::TriggerOutput(AActor* EventInstigator)
{
	if (bIsEnabled)
	{
		OnTriggered.Broadcast(EventInstigator);
	}
}

void ARockGameplayNode::UpdateBaseAndBeam()
{
#if WITH_EDITOR
	if (!GetWorld() || GetWorld()->WorldType != EWorldType::Editor)
	{
		return;
	}

	float MaxBaseLength = 256.7f;
	//
	// // Get sprite's world location
	FVector SpriteLocation = EditorOnly_Sprite->GetComponentLocation();
	//
	// Perform line trace to find ground
	FHitResult HitResult;
	FVector TraceStart = SpriteLocation;
	FVector TraceEnd = TraceStart - FVector(0, 0, MaxBaseLength);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECollisionChannel::ECC_WorldStatic,
		QueryParams
	);
	TArray<AActor*> ActorsToIgnore = {this};
	UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd,
		ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration, HitResult, true,
		FLinearColor::Red, FLinearColor::Green, 1.0f);

	float DistanceToGround = MaxBaseLength;
	if (bHit)
	{
		DistanceToGround = FMath::Min((SpriteLocation - HitResult.Location).Size(), MaxBaseLength);
	}
	float BaseWidth = 0.5f;
	// // Update base component
	EditorOnly_RockBase->SetRelativeLocation(FVector(0.0f, 0.0f, -DistanceToGround), false, nullptr, ETeleportType::TeleportPhysics);
	EditorOnly_RockBase->SetRelativeScale3D(FVector(1, 1, 1));
	//
	float BeamWidth = 1;
	//
	// // Update beam component - place at ground level
	FVector BeamLocation = SpriteLocation - FVector(0, 0, DistanceToGround);
	EditorOnly_Beam->SetWorldLocation(BeamLocation);
	EditorOnly_Beam->SetRelativeScale3D(FVector(BeamWidth, BeamWidth, DistanceToGround));
	//
	// // Rotate beam to be vertical
	// EditorOnly_Beam->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
#endif
}

#if WITH_EDITOR
void ARockGameplayNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// // Get the name of the property that was changed
	// FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	// // Update visuals when transform is modified or relevant properties change
	// if (PropertyName == GET_MEMBER_NAME_CHECKED(AActor, GetRootComponent()) ||
	// 	//PropertyName == GET_MEMBER_NAME_CHECKED(USceneComponent, RelativeLocation) ||
	// 	//PropertyName == GET_MEMBER_NAME_CHECKED(USceneComponent, RelativeRotation) ||
	// 	//PropertyName == TEXT("MaxBaseLength") ||
	// 	PropertyName == TEXT("BeamWidth") ||
	// 	PropertyName == TEXT("BaseWidth") ||
	// 	PropertyName == NAME_None) // NAME_None is passed when multiple properties change at once
	// {
	// 	UpdateBaseAndBeam();
	// }
}

void ARockGameplayNode::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	UpdateBaseAndBeam();
}


#endif
