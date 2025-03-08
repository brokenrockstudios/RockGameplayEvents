// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "DetailCustomization/RockGameplayEventDelegateConnectionsCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "RandomFunctions.h"
#include "Component/RockDelegateConnectorComponent.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Misc/MiscHelperFunctions.h"
#include "Widgets/RockDelegateDropdownWidget.h"
#include "Widgets/RockFunctionDropdownWidget.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "FRockEventDelegateDetails"

TSharedRef<IPropertyTypeCustomization> FRockGameplayEventDelegateConnectionsCustomization::MakeInstance()
{
	return MakeShareable(new FRockGameplayEventDelegateConnectionsCustomization());
}

void FRockGameplayEventDelegateConnectionsCustomization::CustomizeHeader(
	TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& InStructCustomizationUtils)
{
	if (!IModularFeatures::Get().IsModularFeatureAvailable("PropertyAccessEditor"))
	{
		return;
	}
	BindingsHandler = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, Bindings));
	DelegatePropertyNameHandler = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, DelegatePropertyName));
	DelegatePropertyTypeHandler = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, DelegateType));

	if (!BindingsHandler.IsValid() || !DelegatePropertyNameHandler.IsValid())
	{
		return;
	}
	// Get the name from NameHandler
	FName DelegatePropertyName;
	if (DelegatePropertyNameHandler->GetValue(DelegatePropertyName) == FPropertyAccess::Fail)
	{
		DelegatePropertyName = "None";
	}

	uint32 NumElements;
	BindingsHandler->GetNumChildren(NumElements);

	HeaderRow
		.NameContent()
		[
			// Fetch the name from the FRockGameplayEventConnection::DelegatePropertyName
			SNew(STextBlock)
			.Text(this, &FRockGameplayEventDelegateConnectionsCustomization::GetSelectedDelegate)
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(RockCustomizationFunctions::GetArrayTextValue(NumElements))
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				BindingsHandler->CreateDefaultPropertyButtonWidgets()
			]
		];
}

void FRockGameplayEventDelegateConnectionsCustomization::CustomizeChildren(
	TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InStructCustomizationUtils)
{
	if (!BindingsHandler.IsValid() || !DelegatePropertyNameHandler.IsValid())
	{
		return;
	}
	const UClass* PropertyOwnerClass = GetPropertyOwnerClassFromBindingsHandler();
	if (!PropertyOwnerClass)
	{
		return;
	}
	AvailableDelegates.Empty();
	auto delegates = UMiscHelperFunctions::GetDelegatesForActorClass(PropertyOwnerClass);

	for (const FRockDelegateInfo& Delegate : delegates)
	{
		AvailableDelegates.Add(MakeShareable(new FRockDelegateInfo(Delegate)));
	}

	InChildBuilder.AddProperty(DelegatePropertyNameHandler.ToSharedRef())
		.CustomWidget()
		[
			SNew(SRockDelegateDropdownWidget)
			.AvailableDelegates(AvailableDelegates)
			.ContentPadding(FMargin(4.0f, 2.0f))
			.OnDelegateSelected(this, &FRockGameplayEventDelegateConnectionsCustomization::OnMulticastDelegateSelected)
			.ButtonContent()
			[
				SNew(STextBlock)
				.Text(this, &FRockGameplayEventDelegateConnectionsCustomization::GetSelectedDelegate)
			]
		];

	uint32 NumChildren;
	BindingsHandler->GetNumChildren(NumChildren);

	for (uint32 Index = 0; Index < NumChildren; ++Index)
	{
		TSharedPtr<IPropertyHandle> ElementHandle = BindingsHandler->GetChildHandle(Index);
		if (!ElementHandle.IsValid())
		{
			continue;
		}
		TSharedPtr<IPropertyHandle> TargetActorHandle = ElementHandle->GetChildHandle(
			GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, TargetActor));
		TSharedPtr<IPropertyHandle> EventFunctionReferenceHandle = ElementHandle->GetChildHandle(
			GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, FunctionNameToBind));

		TArray<UFunction*>& FunctionList = ElementFunctionListMap.FindOrAdd(TargetActorHandle);

		UpdateFunctionList(TargetActorHandle);
		IDetailPropertyRow& DetailRow = InChildBuilder.AddProperty(ElementHandle.ToSharedRef());
		DetailRow.CustomWidget()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SSplitter)
				+ SSplitter::Slot()
				.Value(SplitterCoefficient)
				.OnSlotResized_Lambda([this](float InValue) { SplitterCoefficient = InValue; })
				[
					SNew(SObjectPropertyEntryBox)
					.PropertyHandle(TargetActorHandle)
					.AllowedClass(AActor::StaticClass())
					.OnObjectChanged_Lambda([this, TargetActorHandle](const FAssetData& AssetData)
					{
						UE_LOG(LogTemp, Warning, TEXT("Object Changed"));
						UpdateFunctionList(TargetActorHandle);
					})
				]
				+ SSplitter::Slot()
				// .Value() // Bind to 1-coefficent? I dunno
				[
					SNew(SRockFunctionDropdownWidget)
						.AvailableFunctions(FunctionList)
						.OnFunctionSelected(this, &FRockGameplayEventDelegateConnectionsCustomization::OnFunctionSelected, ElementHandle)
						.ButtonContent()
					    						[
							SNew(STextBlock)
							.Text(this, &FRockGameplayEventDelegateConnectionsCustomization::GetSelectedFunctionName, ElementHandle)
						]
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				// Is there a way we can prevent the user from adding a bunch of 'empty nodes'
				// or should we complain if they exist later? 
				ElementHandle->CreateDefaultPropertyButtonWidgets()
			]
		];
	}
}


void FRockGameplayEventDelegateConnectionsCustomization::UpdateFunctionList(const TSharedPtr<IPropertyHandle>& TargetActorHandle)
{
	ElementFunctionListMap[TargetActorHandle].Empty();

	const UClass* PropertyOwnerClass = GetPropertyOwnerClassFromBindingsHandler();
	const FText SelectedDelegate2 = GetSelectedDelegate();
	if (!PropertyOwnerClass || SelectedDelegate2.IsEmpty())
	{
		return;
	}

	FMulticastDelegateProperty* MulticastDelegateProperty = FindFProperty<FMulticastDelegateProperty>(PropertyOwnerClass,
		*SelectedDelegate2.ToString());
	if (!MulticastDelegateProperty)
	{
		return;
	}
	auto pprotoTypeFunction = MulticastDelegateProperty->SignatureFunction;

	auto IsCompatibleFunction = [this, pprotoTypeFunction](const UFunction* TestFunction) -> bool
	{
		if (TestFunction && pprotoTypeFunction)
		{
			if (pprotoTypeFunction->IsSignatureCompatibleWith(TestFunction) &&
				FBlueprintEditorUtils::HasFunctionBlueprintThreadSafeMetaData(pprotoTypeFunction) ==
				FBlueprintEditorUtils::HasFunctionBlueprintThreadSafeMetaData(TestFunction))
			{
				return true;
			}
		}
		return false;
	};

	TArray<UFunction*>& Function_List = ElementFunctionListMap.FindOrAdd(TargetActorHandle);
	const AActor* TargetActor = GetActorFromHandle(TargetActorHandle);
	if (TargetActor)
	{
		UClass* ActorClass = TargetActor->GetClass();
		for (TFieldIterator<UFunction> FunctionIter(ActorClass, EFieldIteratorFlags::IncludeSuper); FunctionIter; ++FunctionIter)
		{
			UFunction* TestFunction = *FunctionIter;
			if (IsCompatibleFunction(TestFunction))
			{
				FString FunctionName = TestFunction->GetName();
				Function_List.Add(TestFunction);
			}
		}
	}
}


	
void FRockGameplayEventDelegateConnectionsCustomization::OnMulticastDelegateSelected(TSharedPtr<FRockDelegateInfo> InItem, ESelectInfo::Type arg)
{
	if (DelegatePropertyNameHandler.IsValid())
	{
		DelegatePropertyNameHandler->SetValue(InItem->Name);
	}
	if (DelegatePropertyTypeHandler.IsValid())
	{
		ERockDelegateType type = InItem->bIsSparse ? ERockDelegateType::Sparse : ERockDelegateType::Regular;
		DelegatePropertyTypeHandler->SetValue(static_cast<int32>(type));
	}
}

void FRockGameplayEventDelegateConnectionsCustomization::OnFunctionSelected( UFunction* theFunction, ESelectInfo::Type someType, TSharedPtr<IPropertyHandle> ElementHandle)
{
	FString itemStr = "None";
	if (theFunction)
	{
		itemStr = *theFunction->GetName();
	}
	UE_LOG(LogTemp, Warning, TEXT("OnFunctionSelected Item: %s"), *itemStr);
	const TSharedPtr<IPropertyHandle> EventFunctionReferenceHandle = ElementHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, FunctionNameToBind), false);
	if (!itemStr.IsEmpty() && EventFunctionReferenceHandle.IsValid())
	{
		// Get the selected function name
		const FString SelectedFunctionNameStr(*itemStr);
		const FName SelectedFunctionName(*itemStr);
		if (SelectedFunctionNameStr == "None")
		{
			// Set the function name to none
			FString NoneFunction = "";
			EventFunctionReferenceHandle->SetValue(NoneFunction);
			return;
		}
		EventFunctionReferenceHandle->SetValue(SelectedFunctionName);
	}
}

FText FRockGameplayEventDelegateConnectionsCustomization::GetSelectedDelegate() const
{
	FName DelegateName;
	if (DelegatePropertyNameHandler.IsValid())
	{
		DelegatePropertyNameHandler->GetValue(DelegateName);
	}
	return DelegateName.IsNone() ? FText::FromString("None") : FText::FromName(DelegateName);
}

FText FRockGameplayEventDelegateConnectionsCustomization::GetSelectedFunctionName(TSharedPtr<IPropertyHandle> PropertyHandle) const
{
	const TSharedPtr<IPropertyHandle> EventFunctionReferenceHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, FunctionNameToBind), false);

	if (EventFunctionReferenceHandle.IsValid())
	{
		void* StructData = nullptr;
		if (EventFunctionReferenceHandle->GetValueData(StructData) == FPropertyAccess::Success)
		{
			check(StructData);
			const FName Name = *static_cast<FName*>(StructData);
			return FText::FromName(Name);
		}
	}
	return FText::FromString("");
}

AActor* FRockGameplayEventDelegateConnectionsCustomization::GetActorFromHandle(const TSharedPtr<IPropertyHandle>& PropertyHandle) const
{
	UObject* TargetActorObject = nullptr;
	PropertyHandle->GetValue(TargetActorObject);
	return Cast<AActor>(TargetActorObject);
}

const UClass* FRockGameplayEventDelegateConnectionsCustomization::GetPropertyOwnerClassFromBindingsHandler() const
{
	TArray<UObject*> OutObjects;
	BindingsHandler->GetOuterObjects(OutObjects);
	// There should be only 1 outer, anything else is problematic?
	if (OutObjects.Num() != 1)
	{
		return nullptr;
	}
	URockDelegateConnectorComponent* ConnectorComponent = Cast<URockDelegateConnectorComponent>(OutObjects[0]);
	if (!ConnectorComponent)
	{
		return nullptr;
	}
	const AActor* CompOwner = ConnectorComponent->GetOwner();
	if (!CompOwner)
	{
		return nullptr;
	}
	const UClass* PropertyOwnerClass = CompOwner->GetClass();
	return PropertyOwnerClass;
}
