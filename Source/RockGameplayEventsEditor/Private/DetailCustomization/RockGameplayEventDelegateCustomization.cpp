// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "DetailCustomization/RockGameplayEventDelegateCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "PropertyCustomizationHelpers.h"
#include "Delegate/RockGameplayEventDelegate.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "FRockEventDelegateDetails"

TSharedRef<IPropertyTypeCustomization> FRockGameplayEventDelegateCustomization::MakeInstance()
{
	return MakeShareable(new FRockGameplayEventDelegateCustomization());
}

void FRockGameplayEventDelegateCustomization::CustomizeHeader(
	TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& InStructCustomizationUtils)
{
	if (IModularFeatures::Get().IsModularFeatureAvailable("PropertyAccessEditor"))
	{
		const TSharedPtr<IPropertyHandle> EventListenersHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventDelegate,	EventListeners));
		if (EventListenersHandle.IsValid())
		{
			uint32 NumElements;
			EventListenersHandle->GetNumChildren(NumElements);

			EventListenersHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(
				[&InStructCustomizationUtils]()
				{
					InStructCustomizationUtils.GetPropertyUtilities()->ForceRefresh();
				}));

			HeaderRow
				.NameContent()
				[
					PropertyHandle->CreatePropertyNameWidget()
				]
				.ValueContent()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(GetArrayTextValue(NumElements))
						.Font(IDetailLayoutBuilder::GetDetailFont())
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						EventListenersHandle->CreateDefaultPropertyButtonWidgets()
					]
				];
		}
		else
		{
			HeaderRow
			[
				PropertyHandle->CreatePropertyNameWidget()
			];
		}
	}
}


void FRockGameplayEventDelegateCustomization::CustomizeChildren(
	TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InStructCustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> EventListenersHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventDelegate,
		EventListeners));
	if (!EventListenersHandle.IsValid())
	{
		return;
	}

	uint32 NumChildren;
	EventListenersHandle->GetNumChildren(NumChildren);
	for (uint32 Index = 0; Index < NumChildren; ++Index)
	{
		TSharedPtr<IPropertyHandle> ElementHandle = EventListenersHandle->GetChildHandle(Index);
		if (!ElementHandle.IsValid())
		{
			continue;
		}

		TSharedPtr<IPropertyHandle> TargetActorHandle = ElementHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventListener, TargetActor));
		TSharedPtr<IPropertyHandle> EventFunctionReferenceHandle = ElementHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventListener, EventFunctionReference));

		// Only need to do this once
		if (!PrototypeFunction)
		{
			const FString& PrototypeFunctionName = EventFunctionReferenceHandle->GetMetaData("PrototypeFunction");
			PrototypeFunction = PrototypeFunctionName.IsEmpty() ? nullptr : FindObject<UFunction>(nullptr, *PrototypeFunctionName);
		}

		TArray<TSharedPtr<FString>>& FunctionList = ElementFunctionListMap.FindOrAdd(TargetActorHandle);
		UpdateFunctionList(TargetActorHandle);
		
		IDetailPropertyRow& DetailRow = InChildBuilder.AddProperty(ElementHandle.ToSharedRef());
		DetailRow.CustomWidget()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SObjectPropertyEntryBox)
					.PropertyHandle(TargetActorHandle)
					.AllowedClass(AActor::StaticClass())
					.OnObjectChanged_Lambda([this, TargetActorHandle](const FAssetData& AssetData)
					{
						UpdateFunctionList(TargetActorHandle);
					})
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					// Is there some way we can leverage the default FBlueprintMemberReferenceDetails
					// The main issue is that we need to filter the functions based on the TargetActor

					//EventFunctionReferenceHandle->CreatePropertyNameWidget()
					SNew(SComboBox<TSharedPtr<FString>>)
					.OptionsSource(&FunctionList)
					.OnGenerateWidget_Lambda([](const TSharedPtr<FString>& InItem)
					{
						return SNew(STextBlock).Text(FText::FromString(*InItem));
					})
					.OnSelectionChanged(this, &FRockGameplayEventDelegateCustomization::OnFunctionChanged, ElementHandle)
					[
						SNew(STextBlock)
						.Text(this, &FRockGameplayEventDelegateCustomization::GetSelectedFunctionName, ElementHandle)
					]
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				ElementHandle->CreateDefaultPropertyButtonWidgets()
			]
		];
	}
}

FText FRockGameplayEventDelegateCustomization::GetSelectedFunctionName(TSharedPtr<IPropertyHandle> PropertyHandle) const
{
	const TSharedPtr<IPropertyHandle> TargetActorHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventListener, TargetActor), false);
	const TSharedPtr<IPropertyHandle> EventFunctionReferenceHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventListener, EventFunctionReference), false);

	const AActor* Actor = GetActorFromHandle(TargetActorHandle);
	const UBlueprint* Blueprint = GetBlueprintClassFromActor(Actor);
	if (Blueprint)
	{
		void* StructData = nullptr;
		const FPropertyAccess::Result Result = EventFunctionReferenceHandle->GetValueData(StructData);
		if (Result == FPropertyAccess::Success)
		{
			check(StructData);
			FMemberReference* MemberReference = static_cast<FMemberReference*>(StructData);

			if (UFunction* Function = MemberReference->ResolveMember<UFunction>(Blueprint->SkeletonGeneratedClass))
			{
				return FText::FromName(Function->GetFName());
			}
		}
	}
	return FText::FromString("");
}

void FRockGameplayEventDelegateCustomization::UpdateFunctionList(const TSharedPtr<IPropertyHandle>& TargetActorHandle)
{
	ElementFunctionListMap[TargetActorHandle].Empty();

	TArray<TSharedPtr<FString>>& FunctionList = ElementFunctionListMap.FindOrAdd(TargetActorHandle);

	const AActor* TargetActor = GetActorFromHandle(TargetActorHandle);

	if (TargetActor)
	{
		UClass* ActorClass = TargetActor->GetClass();
		for (TFieldIterator<UFunction> FunctionIter(ActorClass, EFieldIteratorFlags::IncludeSuper); FunctionIter; ++FunctionIter)
		{
			const UFunction* TestFunction = *FunctionIter;
			if (IsCompatibleFunction(TestFunction))
			{
				FString FunctionName = TestFunction->GetName();
				FunctionList.Add(MakeShareable(new FString(FunctionName)));
			}
		}
	}
}

bool FRockGameplayEventDelegateCustomization::IsCompatibleFunction(const UFunction* TestFunction) const
{
	if (TestFunction && PrototypeFunction)
	{
		if (PrototypeFunction->IsSignatureCompatibleWith(TestFunction) &&
			FBlueprintEditorUtils::HasFunctionBlueprintThreadSafeMetaData(PrototypeFunction) == FBlueprintEditorUtils::HasFunctionBlueprintThreadSafeMetaData(TestFunction))
		{
			return true;
		}
	}
	return false;
}


void FRockGameplayEventDelegateCustomization::OnFunctionChanged(
	TSharedPtr<FString> Item,
	ESelectInfo::Type SelectInfo,
	TSharedPtr<IPropertyHandle> ElementHandle) const
{
	const TSharedPtr<IPropertyHandle> TargetActorHandle = ElementHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventListener, TargetActor), false);
	const TSharedPtr<IPropertyHandle> EventFunctionReferenceHandle = ElementHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventListener, EventFunctionReference), false);
	
	if (Item.IsValid() && TargetActorHandle.IsValid() && EventFunctionReferenceHandle.IsValid())
	{
		// Get the selected function name
		const FString SelectedFunctionNameStr(*Item);
		const FName SelectedFunctionName(*Item);

		const AActor* Actor = GetActorFromHandle(TargetActorHandle);
		if (!Actor)
		{
			// Failed to cast the target actor to AActor.
			return;
		}

		const UClass* ScopeClass = Actor->GetClass();
		if (ScopeClass == nullptr)
		{
			// Failed to retrieve the class of the target actor.
			return;
		}

		const UFunction* Function = ScopeClass->FindFunctionByName(SelectedFunctionName);
		if (Function == nullptr)
		{
			// Failed to find the function by name.
			return;
		}

		const UBlueprint* Blueprint = GetBlueprintClassFromActor(Actor);
		if (Blueprint == nullptr)
		{
			// Failed to cast the class generated by the blueprint.
			return;
		}

		void* StructData = nullptr;
		const FPropertyAccess::Result Result = EventFunctionReferenceHandle->GetValueData(StructData);
		if (Result == FPropertyAccess::Success)
		{
			check(StructData)
			FMemberReference* MemberReference = static_cast<FMemberReference*>(StructData);
			const bool bSelfContext = (Blueprint->GeneratedClass != nullptr && Blueprint->GeneratedClass->IsChildOf(ScopeClass)) ||
				(Blueprint->SkeletonGeneratedClass != nullptr && Blueprint->SkeletonGeneratedClass->IsChildOf(ScopeClass));

			MemberReference->SetFromField<UFunction>(Function, bSelfContext);
		}
	}
}

AActor* FRockGameplayEventDelegateCustomization::GetActorFromHandle(const TSharedPtr<IPropertyHandle>& PropertyHandle) const
{
	UObject* TargetActorObject = nullptr;
	PropertyHandle->GetValue(TargetActorObject);
	return Cast<AActor>(TargetActorObject);
}

FText FRockGameplayEventDelegateCustomization::GetArrayTextValue(uint32 NumElements) const
{
	return NumElements > 1
		? FText::Format(LOCTEXT("NumArrayItemsFmt", "{0} Array elements"), FText::AsNumber(NumElements))
		: FText::Format(LOCTEXT("SingleArrayItemFmt", "{0} Array element"), FText::AsNumber(NumElements));
}

UBlueprint* FRockGameplayEventDelegateCustomization::GetBlueprintClassFromActor(const AActor* Actor) const
{
	if (Actor)
	{
		UClass* ActorClass = Actor->GetClass();
		if (ActorClass)
		{
			return Cast<UBlueprint>(ActorClass->ClassGeneratedBy);
		}
	}
	return nullptr;
}


#undef LOCTEXT_NAMESPACE
