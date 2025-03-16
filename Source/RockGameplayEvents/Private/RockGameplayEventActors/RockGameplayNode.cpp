// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode.h"

#include "Component/RockDelegateConnectorComponent.h"
#include "Components/BillboardComponent.h"


ARockGameplayNode::ARockGameplayNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	// set collision ignore all
	MeshComponent->SetMobility(EComponentMobility::Movable);
	RootComponent = MeshComponent;


#if WITH_EDITORONLY_DATA
	if (!IsRunningCommandlet() && !IsRunningDedicatedServer())
	{
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BaseMesh;
			ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BeamMesh;
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;
			FName Sprite_ID_Effects;
			FText Sprite_NAME_Effects;

			FConstructorStatics()
				: BaseMesh(TEXT("/RockGameplayEvents/Rock_Base_2")),
				  BeamMesh(TEXT("/RockGameplayEvents/Rock_Beam"))
				  , SpriteTextureObject(TEXT("/RockGameplayEvents/Bubble_Empty"))
				  , Sprite_ID_Effects(TEXT("Effects"))
				  , Sprite_NAME_Effects(NSLOCTEXT("SpriteCategory", "Effects", "Effects1"))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		EditorOnly_RockBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RockBase"));

		// THIS IS UGLY. Can we spin this off to like a custom editor component or something?
		if (EditorOnly_RockBase)
		{
			EditorOnly_RockBase->SetStaticMesh(ConstructorStatics.BaseMesh.Get());
			EditorOnly_RockBase->SetMobility(EComponentMobility::Movable);
			FVector RockBaseLocation = EditorOnly_RockBase->GetRelativeLocation();
			EditorOnly_RockBase->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			EditorOnly_RockBase->SetRelativeScale3D(FVector(1, 1, 1));
			EditorOnly_RockBase->bHiddenInGame = true;
			EditorOnly_RockBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			EditorOnly_RockBase->SetupAttachment(MeshComponent);
			EditorOnly_RockBase->bReceivesDecals = false;

			EditorOnly_RockBase->SetUsingAbsoluteScale(true);
		}
		EditorOnly_Sprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
		if (EditorOnly_Sprite)
		{
			EditorOnly_Sprite->SetSprite(ConstructorStatics.SpriteTextureObject.Get());
			EditorOnly_Sprite->SetMobility(EComponentMobility::Movable);
			EditorOnly_Sprite->SetRelativeLocation(FVector(0.0f, 0.0f, 160.0f));
			EditorOnly_Sprite->SetRelativeScale3D(FVector(1, 1, 1));
			EditorOnly_Sprite->bHiddenInGame = true;
			EditorOnly_Sprite->bIsScreenSizeScaled = true;
			EditorOnly_Sprite->SpriteInfo.Category = ConstructorStatics.Sprite_ID_Effects;
			EditorOnly_Sprite->SpriteInfo.DisplayName = ConstructorStatics.Sprite_NAME_Effects;
			EditorOnly_Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			EditorOnly_Sprite->SetupAttachment(EditorOnly_RockBase);
			EditorOnly_Sprite->bReceivesDecals = false;
			EditorOnly_Sprite->SetUsingAbsoluteScale(true);
		}
		EditorOnly_Beam = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beam"));
		if (EditorOnly_Beam)
		{
			EditorOnly_Beam->SetMobility(EComponentMobility::Movable);
			EditorOnly_Beam->SetStaticMesh(ConstructorStatics.BeamMesh.Get());
			EditorOnly_Beam->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			EditorOnly_Beam->SetRelativeScale3D(FVector(1, 1, 160));
			EditorOnly_Beam->bHiddenInGame = true;
			EditorOnly_Beam->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			EditorOnly_Beam->SetupAttachment(EditorOnly_RockBase);
			EditorOnly_Beam->bReceivesDecals = false;
			EditorOnly_Beam->SetUsingAbsoluteScale(true);
		}
	}
#endif

	GameplayConnector = CreateDefaultSubobject<URockDelegateConnectorComponent>(TEXT("DelegateConnector"));
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

void ARockGameplayNode::ResetNode(AActor* EventInstigator)
{
	// Nothing to reset in base node
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
	// if (!GetWorld() || GetWorld()->WorldType != EWorldType::Editor)
	// {
	// 	return;
	// }
	//
	// float MaxBaseLength = 384.84f;
	//
	// // Get sprite's world location
	// FVector SpriteLocation = EditorOnly_Sprite->GetComponentLocation();
	// // Perform line trace to find ground
	// FHitResult HitResult;
	// const FVector TraceStart = SpriteLocation;
	// const FVector TraceEnd = TraceStart - FVector(0, 0, MaxBaseLength);
	//
	// FCollisionQueryParams QueryParams;
	// QueryParams.AddIgnoredActor(this);
	//
	// const bool bHit = GetWorld()->LineTraceSingleByChannel(
	// 	HitResult,
	// 	TraceStart,
	// 	TraceEnd,
	// 	ECollisionChannel::ECC_WorldStatic,
	// 	QueryParams
	// );
	// // TArray<AActor*> ActorsToIgnore = {this};
	// // UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd,ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,	EDrawDebugTrace::ForDuration, HitResult, true,		FLinearColor::Red, FLinearColor::Green, 1.0f);
	// float DistanceToGround = MaxBaseLength;
	// if (bHit)
	// {
	// 	DistanceToGround = FMath::Min((SpriteLocation - HitResult.Location).Size(), MaxBaseLength);
	// }
	// // Update base component
	// EditorOnly_RockBase->SetRelativeLocation(FVector(0.0f, 0.0f, -DistanceToGround), false, nullptr, ETeleportType::TeleportPhysics);
	// EditorOnly_RockBase->SetRelativeScale3D(FVector(1, 1, 1));
	//
	// // Update beam component - place at ground level
	// FVector BeamLocation = SpriteLocation - FVector(0, 0, DistanceToGround);
	// EditorOnly_Beam->SetWorldLocation(BeamLocation);
	// EditorOnly_Beam->SetRelativeScale3D(FVector(1, 1, DistanceToGround));
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
	//UpdateBaseAndBeam();
}


#endif
