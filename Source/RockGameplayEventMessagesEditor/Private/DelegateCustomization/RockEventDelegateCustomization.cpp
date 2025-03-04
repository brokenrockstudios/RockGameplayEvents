// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "DelegateCustomization/RockEventDelegateCustomization.h"

#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"
#include "Delegate/RockGameplayEventDelegateData.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "FRockEventDelegateDetails"

TSharedRef<IPropertyTypeCustomization> FRockEventDelegateDetails::MakeInstance()
{
	return MakeShareable(new FRockEventDelegateDetails());
}

FText FRockEventDelegateDetails::GetSelectedFunctionName() const
{
	FMemberReference MemberReference;
	void* StructData = nullptr;
	if (EventFunctionReferenceHandle->GetValueData(StructData) == FPropertyAccess::Result::Success)
	{
		auto MemberReferenceData = static_cast<FMemberReference*>(StructData);
		return FText::FromString(MemberReferenceData->GetMemberName().ToString());
	}
	return FText::FromString("Failed");
}


TSharedRef<SWidget> FRockEventDelegateDetails::MakeFunctionItemWidget(const TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

bool FRockEventDelegateDetails::IsCompatibleFunction(const UFunction* TestFunction) const
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

void FRockEventDelegateDetails::UpdateFunctionList()
{
	FunctionList.Empty();

	UObject* TargetActorObject = nullptr;
	TargetActorHandle->GetValue(TargetActorObject);
	const AActor* TargetActor = Cast<AActor>(TargetActorObject);

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

void FRockEventDelegateDetails::OnTargetActorChanged(const FAssetData& NewAssetData)
{
	UpdateFunctionList();
}


void FRockEventDelegateDetails::CustomizeHeader(
	TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& InStructCustomizationUtils)
{
	if (IModularFeatures::Get().IsModularFeatureAvailable("PropertyAccessEditor"))
	{
		TargetActorHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventDelegateData, TargetActor));
		EventFunctionReferenceHandle = PropertyHandle->
			GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventDelegateData, EventFunctionReference));

		const FString& PrototypeFunctionName = EventFunctionReferenceHandle->GetMetaData("PrototypeFunction");
		PrototypeFunction = PrototypeFunctionName.IsEmpty() ? nullptr : FindObject<UFunction>(nullptr, *PrototypeFunctionName);

		UpdateFunctionList();
		HeaderRow
			.NameContent()
			[
				PropertyHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			.MinDesiredWidth(500.f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SObjectPropertyEntryBox)
					.PropertyHandle(TargetActorHandle)
					.AllowedClass(AActor::StaticClass())
					.OnObjectChanged(this, &FRockEventDelegateDetails::OnTargetActorChanged)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					// TODO: FoodForThought Is there a better way to leverage the BlueprintMemberReferenceCustomization
					SNew(SComboBox<TSharedPtr<FString>>)
					.OptionsSource(&FunctionList)
					.OnGenerateWidget(this, &FRockEventDelegateDetails::MakeFunctionItemWidget)
					.OnSelectionChanged(this, &FRockEventDelegateDetails::OnFunctionSelected)
					.Content()
					[
						SNew(STextBlock)
						.Text(this, &FRockEventDelegateDetails::GetSelectedFunctionName)
					]
				]
			];
	}
}

void FRockEventDelegateDetails::OnFunctionSelected(const TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	if (Item.IsValid())
	{
		FString SelectedFunctionName = *Item;

		// THIS FEELS WRONG/HACKY? How do we get the proper FGuid
		FGuid SelectedFunctionGuid = FGuid();

		UObject* ActorObject = nullptr;
		TSubclassOf<UClass> ScopeClass;
		if (TargetActorHandle->GetValue(ActorObject) == FPropertyAccess::Result::Success)
		{
			AActor* Actor = Cast<AActor>(ActorObject);
			if (Actor)
			{
				ScopeClass = Actor->GetClass();
			}
		}

		// THIS FEELS HACKY
		FMemberReference MemberReference;
		FName ItemName = FName(*SelectedFunctionName);
		MemberReference.SetDirect(ItemName, SelectedFunctionGuid, ScopeClass, false);

		// Do appropriate steps to be able to call SetvValue on the Reference Handle
		// MORE UGLY HACK, LOOK AWAY
		void* StructData = nullptr;
		if (EventFunctionReferenceHandle->GetValueData(StructData) == FPropertyAccess::Result::Success)
		{
			auto MemberReferenceData = static_cast<FMemberReference*>(StructData);
			*MemberReferenceData = MemberReference;
		}
	}
}

#undef LOCTEXT_NAMESPACE
