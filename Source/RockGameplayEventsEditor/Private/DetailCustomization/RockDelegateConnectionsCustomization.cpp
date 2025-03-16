// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "DetailCustomization/RockDelegateConnectionsCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "PropertyCustomizationHelpers.h"
#include "RandomFunctions.h"
#include "RockEventsEditorFunctionLibrary.h"
#include "Component/RockDelegateConnectorComponent.h"
#include "Misc/MiscHelperFunctions.h"
#include "Widgets/RockDelegateDropdownWidget.h"
#include "Widgets/RockFunctionDropdownWidget.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "FRockEventDelegateDetails"

TSharedRef<IPropertyTypeCustomization> FRockDelegateConnectionsCustomization::MakeInstance()
{
	return MakeShareable(new FRockDelegateConnectionsCustomization());
}

void FRockDelegateConnectionsCustomization::CustomizeHeader(
	TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& InStructCustomizationUtils)
{
	//if (!IModularFeatures::Get().IsModularFeatureAvailable("PropertyAccessEditor"))
	// Was used in BlueprintMemberReferenceCustomization for PropertyAccessEditor.MakePropertyBindingWidget(Blueprint, Args)
	MyPropertyHandle = InPropertyHandle;
	CacheData();
	FName DelegatePropertyName;
	if (DelegatePropertyNameHandler->GetValue(DelegatePropertyName) == FPropertyAccess::Fail)
	{
		DelegatePropertyName = "None";
	}
	uint32 NumElements;
	BindingsHandler->GetNumChildren(NumElements);
	const TSharedPtr<IPropertyUtilities> PropertyUtilities = InStructCustomizationUtils.GetPropertyUtilities();
	BindingsHandler->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([PropertyUtilities]()
	{
		// Without this, it doesn't certain aspects of the array properly. There is probably a more concise fix, but this works for now. 
		PropertyUtilities->ForceRefresh();
	}));

	HeaderRow
		.NameContent()
		[
			// Fetch the name from the FRockGameplayEventConnection::DelegatePropertyName
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(this, &FRockDelegateConnectionsCustomization::GetSelectedDelegate)
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(FMargin(4.0f, 0.0f))
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush("Icons.Info"))
				.ToolTipText(this, &FRockDelegateConnectionsCustomization::GetDelegateParameterList)
			]
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

void FRockDelegateConnectionsCustomization::CustomizeChildren(
	TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InStructCustomizationUtils)
{
	PropUtils = InStructCustomizationUtils.GetPropertyUtilities().Get();

	InChildBuilder.AddProperty(DelegatePropertyNameHandler.ToSharedRef())
		.CustomWidget()
		[
			SAssignNew(Vert, SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(FMargin(0.0f, 4, 0, 4.0f))
			[
				SNew(SRockDelegateDropdownWidget)
				.AvailableDelegates(AvailableDelegates)
				.ContentPadding(FMargin(4.0f, 2.0f))
				.OnDelegateSelected(this, &FRockDelegateConnectionsCustomization::OnMulticastDelegateSelected)
				.ButtonContent()
				[
					SNew(STextBlock)
					.Text(this, &FRockDelegateConnectionsCustomization::GetSelectedDelegate)
				]
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
				.OnSlotResized_Lambda([this](float InValue) { SplitterCoefficient = InValue; })
				.Value(SplitterCoefficient)
				[
					SNew(SObjectPropertyEntryBox)
					.PropertyHandle(TargetActorHandle)
					.AllowedClass(AActor::StaticClass())
					.OnObjectChanged(this, &FRockDelegateConnectionsCustomization::OnTargetActorSelected, ElementHandle)
					//, TargetActorHandle, EventFunctionReferenceHandle)
				]
				+ SSplitter::Slot()
				// .Value() // Bind to 1-coefficent? I dunno
				[
					SNew(SRockFunctionDropdownWidget)
					.AvailableFunctions(FunctionList)
					// Should be replaced with something to update if viable match was found?
					.EventFunctionReferenceHandle(EventFunctionReferenceHandle)
					.OnFunctionSelected(this, &FRockDelegateConnectionsCustomization::OnFunctionSelected, ElementHandle)
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

void FRockDelegateConnectionsCustomization::UpdateFunctionList(const TSharedPtr<IPropertyHandle>& TargetActorHandle)
{
	if (TArray<UFunction*>* ListMap = ElementFunctionListMap.Find(TargetActorHandle))
	{
		ListMap->Empty();
	}

	if (CachedActorClass == nullptr)
	{
		// We don't have a valid actor class to search for functions
		return;
	}

	FName DelegateName;
	if (DelegatePropertyNameHandler.IsValid())
	{
		DelegatePropertyNameHandler->GetValue(DelegateName);
	}
	if (DelegateName == NAME_None)
	{
		return;
	}

	const FString SelectedDelegateStr = DelegateName.ToString();

	TObjectPtr<UFunction> DelegateSignatureFunction;
	if (const FMulticastDelegateProperty* MulticastDelegateProperty = FindFProperty<FMulticastDelegateProperty>(CachedActorClass,
		*SelectedDelegateStr))
	{
		DelegateSignatureFunction = MulticastDelegateProperty->SignatureFunction;
	}
	else if (const FDelegateProperty* DelegateProperty = FindFProperty<FDelegateProperty>(CachedActorClass, *SelectedDelegateStr))
	{
		DelegateSignatureFunction = DelegateProperty->SignatureFunction;
	}
	else
	{
		UE_LOG(LogRockGameplayEvents, Warning, TEXT("Unknown FProperty: %s"), *SelectedDelegateStr);
		return;
	}
	TArray<UFunction*>& Function_List = ElementFunctionListMap.FindOrAdd(TargetActorHandle);
	const AActor* TargetActor = GetActorFromHandle(TargetActorHandle);
	if (TargetActor)
	{
		const UClass* ActorClass = TargetActor->GetClass();
		for (TFieldIterator<UFunction> FunctionIter(ActorClass, EFieldIteratorFlags::IncludeSuper); FunctionIter; ++FunctionIter)
		{
			UFunction* TestFunction = *FunctionIter;
			if (URockEventsEditorFunctionLibrary::IsFunctionsCompatible(TestFunction, DelegateSignatureFunction))
			{
				Function_List.Add(TestFunction);
			}
		}
	}
}

void FRockDelegateConnectionsCustomization::OnTargetActorSelected(
	const FAssetData& asset_data, TSharedPtr<IPropertyHandle> ElementHandle)
{
	const TSharedPtr<IPropertyHandle> TargetActorHandle = ElementHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, TargetActor), false);
	const TSharedPtr<IPropertyHandle> EventFunctionReferenceHandle = ElementHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, FunctionNameToBind), false);

	if (AActor* TargetActor = GetActorFromHandle(TargetActorHandle))
	{
		UpdateFunctionList(TargetActorHandle);
	}
	else
	{
		EventFunctionReferenceHandle->SetValue(FName("None"));
	}
}


void FRockDelegateConnectionsCustomization::OnMulticastDelegateSelected(TSharedPtr<FRockDelegateInfo> InItem, ESelectInfo::Type arg)
{
	if (MyPropertyHandle.IsValid() && CachedConnection)
	{
		MyPropertyHandle->NotifyPreChange();
		{
			CachedConnection->DelegatePropertyName = *InItem->Name;
			FString FunctionString = UMiscHelperFunctions::BuildFunctionParameterString(InItem->SignatureFunction, true, true);
			CachedConnection->DelegateParameterList = FunctionString;
			CachedConnection->DelegateType = InItem->DelegateType;
		}
		MyPropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
		MyPropertyHandle->NotifyFinishedChangingProperties();
	}
}

void FRockDelegateConnectionsCustomization::OnFunctionSelected(
	const UFunction* SelectedFunction, ESelectInfo::Type someType, TSharedPtr<IPropertyHandle> ElementHandle) const
{
	FString itemStr = "None";
	if (SelectedFunction)
	{
		itemStr = *SelectedFunction->GetName();
	}
	const TSharedPtr<IPropertyHandle> TargetActorHandle = ElementHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, TargetActor), false);
	const TSharedPtr<IPropertyHandle> EventFunctionReferenceHandle = ElementHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, FunctionNameToBind), false);

	// Get Actor* from TargetActorHandle
	AActor* TargetActor = GetActorFromHandle(TargetActorHandle);
	if (!TargetActor)
	{
		UE_LOG(LogRockGameplayEvents, Warning, TEXT("OnFunctionSelected TargetActor: not valid"));
		return;
	}

	if (!itemStr.IsEmpty() && EventFunctionReferenceHandle.IsValid())
	{
		const FName SelectedFunctionName(*itemStr);
		EventFunctionReferenceHandle->SetValue(SelectedFunctionName);
	}

	// We are done setting the outgoing. Let's update the incoming connection of the target actor
	if (itemStr.IsEmpty() || itemStr == "None")
	{
		// We have nothing left to do
		return;
	}

	URockDelegateConnectorComponent* ConnectorComponent = TargetActor->GetComponentByClass<URockDelegateConnectorComponent>();
	// If the connector component doesn't exist on the target actor, create it
	if (!ConnectorComponent)
	{
		ConnectorComponent = NewObject<URockDelegateConnectorComponent>(TargetActor);
		ConnectorComponent->RegisterComponent();
		TargetActor->AddInstanceComponent(ConnectorComponent);
	}

	// Add the incoming connection from current actor. 
	if (ConnectorComponent && DelegatePropertyNameHandler.IsValid())
	{
		FName DelegateName;
		if (DelegatePropertyNameHandler->GetValue(DelegateName) == FPropertyAccess::Success)
		{
			const FRockGameplayIncomingConnection IncomingConnection(CachedActor.Get(), DelegateName);
			ConnectorComponent->IncomingConnections.Add(IncomingConnection);
		}
	}
}

FText FRockDelegateConnectionsCustomization::GetSelectedDelegate() const
{
	FName DelegateName;
	if (DelegatePropertyNameHandler.IsValid())
	{
		DelegatePropertyNameHandler->GetValue(DelegateName);
	}
	return DelegateName.IsNone() ? FText::FromString("None") : FText::FromName(DelegateName);
}

FText FRockDelegateConnectionsCustomization::GetDelegateParameterList() const
{
	FName DelegateName;
	if (DelegatePropertyNameHandler.IsValid())
	{
		DelegatePropertyNameHandler->GetValue(DelegateName);
	}
	FString DelegateParameterListStr;
	if (DelegateParameterListHandler.IsValid())
	{
		DelegateParameterListHandler->GetValue(DelegateParameterListStr);
	}
	return FText::FromString(DelegateName.ToString() + DelegateParameterListStr);
}

AActor* FRockDelegateConnectionsCustomization::GetActorFromHandle(const TSharedPtr<IPropertyHandle>& PropertyHandle) const
{
	UObject* TargetActorObject = nullptr;
	PropertyHandle->GetValue(TargetActorObject);
	return Cast<AActor>(TargetActorObject);
}

void FRockDelegateConnectionsCustomization::CacheData()
{
	BindingsHandler = MyPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, Bindings));
	DelegatePropertyNameHandler = MyPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, DelegatePropertyName));
	DelegateParameterListHandler = MyPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, DelegateParameterList));
	DelegateTypeHandler = MyPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, DelegateType));

	void* RawData = nullptr;
	if (MyPropertyHandle->GetValueData(RawData) == FPropertyAccess::Success)
	{
		CachedConnection = static_cast<FRockGameplayEventConnection*>(RawData);
	}

	CachedOuterObject = nullptr;
	CachedActor = nullptr;
	CachedActorComponent = nullptr;
	CachedActorClass = nullptr;
	TArray<UObject*> OutObjects;
	MyPropertyHandle->GetOuterObjects(OutObjects);
	if (OutObjects.Num() != 1)
	{
		UE_LOG(LogRockGameplayEvents, Error, TEXT("CacheData failed fetching outer object. %d !=1 "), OutObjects.Num());
		return;
	}
	CachedOuterObject = OutObjects[0];
	CachedActorComponent = Cast<URockDelegateConnectorComponent>(CachedOuterObject.Get());
	if (!CachedActorComponent.Get())
	{
		UE_LOG(LogRockGameplayEvents, Error, TEXT("CacheData failed fetching URockDelegateConnectorComponent"));
		return;
	}
	CachedActor = CachedActorComponent->GetOwner();
	if (!Cast<AActor>(CachedActor.Get()))
	{
		UE_LOG(LogRockGameplayEvents, Error, TEXT("CacheData failed fetching Owner"));
		return;
	}
	CachedActorClass = CachedActor->GetClass();
	if (!CachedActorClass)
	{
		UE_LOG(LogRockGameplayEvents, Error, TEXT("CacheData failed fetching OwnerClass"));
		return;
	}

	AvailableDelegates.Empty();
	auto delegates = UMiscHelperFunctions::GetDelegatesForActorClass(CachedActorClass);
	for (const FRockDelegateInfo& Delegate : delegates)
	{
		AvailableDelegates.Add(MakeShareable(new FRockDelegateInfo(Delegate)));
	}
}
