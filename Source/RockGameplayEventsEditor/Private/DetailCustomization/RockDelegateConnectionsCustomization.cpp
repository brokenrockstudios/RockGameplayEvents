// Copyright Broken Rock Studios LLC. All Rights Reserved.
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

	HeaderRow
		.NameContent()
		[
			// Fetch the name from the FRockGameplayEventConnection::DelegatePropertyName
			SNew(STextBlock)
			.Text(this, &FRockDelegateConnectionsCustomization::GetSelectedDelegate)
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

void FRockDelegateConnectionsCustomization::CustomizeChildren(
	TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InStructCustomizationUtils)
{
	PropUtils = InStructCustomizationUtils.GetPropertyUtilities().Get();

	// InStructCustomizationUtils.GetPropertyUtilities()
	AvailableDelegates.Empty();
	auto delegates = UMiscHelperFunctions::GetDelegatesForActorClass(CachedActorClass);
	DelegateParameterList = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, DelegateParameterList));
	for (const FRockDelegateInfo& Delegate : delegates)
	{
		AvailableDelegates.Add(MakeShareable(new FRockDelegateInfo(Delegate)));
	}


	InChildBuilder.AddProperty(DelegatePropertyNameHandler.ToSharedRef())
		.CustomWidget()
		[
			SAssignNew(Vert, SVerticalBox)
			 + SVerticalBox::Slot()
			 .FillHeight(1)
			 [
			 	SAssignNew(DelegateParamListRef, STextBlock)
			 	.Text(this, &FRockDelegateConnectionsCustomization::GetDelegateParameterList)
			 ]
			 + SVerticalBox::Slot()
			 .AutoHeight()
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
	InChildBuilder.AddProperty(DelegateParameterList.ToSharedRef())
		.Visibility(EVisibility::Visible)
		.CustomWidget()
		[
			SAssignNew(DelegateParamListRef, STextBlock)
			.Text(this, &FRockDelegateConnectionsCustomization::GetDelegateParameterList)
			//.Text(this, &FRockDelegateConnectionsCustomization::GetSelectedDelegate)
			.Font(IDetailLayoutBuilder::GetDetailFont())
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
	ElementFunctionListMap[TargetActorHandle].Empty();

	const FText SelectedDelegateText = GetSelectedDelegate();
	const FString SelectedDelegateStr = SelectedDelegateText.ToString();

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
		// This happens when Actor is empty?
		UE_LOG(LogRockGameplayEvents, Warning, TEXT("Unknown FProperty"));
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
	//auto DelegateParameterList = MyPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, DelegateParameterList));
	FScopedTransaction Transaction(LOCTEXT("YourOperation", "Your Operation"));
	if (DelegateParameterList.IsValid() && DelegatePropertyNameHandler.IsValid())
	{

		FString FunctionString = InItem->GetSignatureFunctionString();


		// DelegateParameterList->NotifyPreChange();
		// TArray<void*> rawDataArray;
		// DelegateParameterList->AccessRawData(rawDataArray);
		// FString* StrPtr = static_cast<FString*>(rawDataArray[0]);
		// check(StrPtr);
		// *StrPtr = FunctionString;		
		// DelegatePropertyNameHandler->SetValue(InItem->Name);
		// DelegateParameterList->NotifyPostChange(EPropertyChangeType::ValueSet);


		// void* RawData;
		// MyPropertyHandle->GetValueData(RawData);
		// CachedConnection = static_cast<FRockGameplayEventConnection*>(RawData);
		void* RawData;
		if (MyPropertyHandle->GetValueData(RawData) == FPropertyAccess::Success)
		{
			check(RawData);
			FRockGameplayEventConnection* CachedConnection = static_cast<FRockGameplayEventConnection*>(RawData);
			
			DelegateParameterList->NotifyPreChange();
			DelegatePropertyNameHandler->NotifyPreChange();
			// Get the raw pointer
			CachedConnection->DelegatePropertyName = *InItem->Name;
			CachedConnection->DelegateParameterList = FunctionString;
			CachedConnection->DelegateType = InItem->DelegateType;
			DelegateParameterList->NotifyPostChange(EPropertyChangeType::ValueSet);
			DelegatePropertyNameHandler->NotifyPostChange(EPropertyChangeType::ValueSet);
		}


		
		
		//UE_LOG(LogRockGameplayEvents, Warning, TEXT("OnMulticastDelegateSelected1 %s"), *InItem->GetSignatureFunctionString());
		//FPropertyAccess::Result result = DelegateParameterList->SetValue(FunctionString, EPropertyValueSetFlags::NotTransactable);
		
		//DelegateParameterList->NotifyPostChange();
		//UE_LOG(LogRockGameplayEvents, Warning, TEXT("OnMulticastDelegateSelected2 %s"), result == FPropertyAccess::Success ? TEXT("Success") : TEXT("Fail"));
		
		
		PropUtils->ForceRefresh();
		
		
	}
	if (DelegateTypeHandler.IsValid())
	{
		int32 DelegateFlags = static_cast<int32>(InItem->DelegateType);
		DelegateTypeHandler->SetValue(DelegateFlags);
	}
}

void FRockDelegateConnectionsCustomization::OnFunctionSelected(
	UFunction* theFunction, ESelectInfo::Type someType, TSharedPtr<IPropertyHandle> ElementHandle)
{
	FString itemStr = "None";
	if (theFunction)
	{
		itemStr = *theFunction->GetName();
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
	}

	// In case we are changing an existing function, we need to remove the old one
	FName PreviousFunctionName;
	if (EventFunctionReferenceHandle.IsValid())
	{
		FString PrevFunctionStr;
		EventFunctionReferenceHandle->GetValue(PrevFunctionStr);
		PreviousFunctionName = *PrevFunctionStr;
	}

	URockDelegateConnectorComponent* ConnectorComponent = TargetActor->GetComponentByClass<URockDelegateConnectorComponent>();

	const bool bIsSettingToNone = itemStr.IsEmpty() || itemStr == "None";

	if (bIsSettingToNone)
	{
		if (ConnectorComponent && !PreviousFunctionName.IsNone())
		{
			// for (int32 i = ConnectorComponent->IncomingConnections.Num() - 1; i >= 0; --i)
			// {
			// 	if (ConnectorComponent->IncomingConnections[i].SourceActor == SourceActor
			// 		&& ConnectorComponent->IncomingConnections[i].DelegatePropertyName == Delegate)
			// 	{
			// 		ConnectorComponent->IncomingConnections.RemoveAt(i);
			// 	}
			// }

			// Remove the incoming connection
			//FRockGameplayIncomingConnection incomingConnection{TargetActor, PreviousFunctionName};
			//ConnectorComponent->IncomingConnections.Remove(incomingConnection);
		}
	}

	// We are wanting to add a URockDelegateConnectorComponent to the actor
	if (!ConnectorComponent)
	{
		ConnectorComponent = NewObject<URockDelegateConnectorComponent>(TargetActor);
		ConnectorComponent->RegisterComponent();
		TargetActor->AddInstanceComponent(ConnectorComponent);
	}

	if (!itemStr.IsEmpty() && EventFunctionReferenceHandle.IsValid())
	{
		const FName SelectedFunctionName(*itemStr);
		EventFunctionReferenceHandle->SetValue(SelectedFunctionName);

		// We could update these later via ASYNC?
		const FRockGameplayIncomingConnection incomingConnection{TargetActor, SelectedFunctionName};
		ConnectorComponent->IncomingConnections.Add(incomingConnection);
	}
	else
	{
		UE_LOG(LogRockGameplayEvents, Warning, TEXT("OnFunctionSelected Item: not valid"));
	}

	// Kick off Async Action to update ALL the incoming connections
	// Clear them all out and regenerate them?
	// Can Optimize it later.
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
	// DelegateParameterList = MyPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, DelegateParameterList));
	
	FString DelegateParameterListStr;
	if (DelegateParameterList.IsValid())
	{
		DelegateParameterList->GetValue(DelegateParameterListStr);
	}
	else
	{
		UE_LOG(LogRockGameplayEvents, Warning, TEXT("GetDelegateParameterList ERR"));
	}
	// UE_LOG(LogRockGameplayEvents, Warning, TEXT("GetDelegateParameterList %s"), *DelegateParameterListStr);
	return FText::FromString(DelegateParameterListStr);
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
	DelegateParameterList = MyPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, DelegateParameterList));
	DelegateTypeHandler = MyPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FRockGameplayEventConnection, DelegateType));

	void* RawData;
	MyPropertyHandle->GetValueData(RawData);
	CachedConnection = static_cast<FRockGameplayEventConnection*>(RawData);	
	// = static_cast<FRockGameplayEventConnection*>();

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
}

// if (SelectedFunctionNameStr == "None")
// {
// 	// Set the function name to none
// 	FString NoneFunction = "";
// 	EventFunctionReferenceHandle->SetValue(NoneFunction);
// 	UE_LOG(LogRockGameplayEvents, Warning, TEXT("OnFunctionSelected Item: None"));
// 	return;
// }


//const void* ValueAddr = nullptr;
//const FMulticastScriptDelegate * value = MulticastDelegateProperty->GetMulticastDelegate(ValueAddr);

// if (const FDelegateProperty* CastProp = CastField<FDelegateProperty>(Prop))
// {
// 	const FScriptDelegate* Value = CastProp->GetPropertyValuePtr(ValueAddr);
// 	OutPyObj = (PyObject*)FPyWrapperDelegateFactory::Get().CreateInstance(CastProp->SignatureFunction, (FScriptDelegate*)Value, OwnerContext, ConversionMethod);
// 	return FPyConversionResult::Success();
// }
//
// if (const FMulticastDelegateProperty* CastProp = CastField<FMulticastDelegateProperty>(Prop))
// {
// 	const FMulticastScriptDelegate* Value = CastProp->GetMulticastDelegate(ValueAddr);
// 	OutPyObj = (PyObject*)FPyWrapperMulticastDelegateFactory::Get().CreateInstance(CastProp->SignatureFunction, (FMulticastScriptDelegate*)Value, OwnerContext, ConversionMethod);
// 	return FPyConversionResult::Success();
// }

//FDelegateProperty* BindableProperty = FindFProperty<FDelegateProperty>(TargetWidget->GetClass(), FName(*( PropertyName.ToString() + TEXT("Delegate") )));
//FDelegateProperty* EventProperty = FindFProperty<FDelegateProperty>(TargetWidget->GetClass(), PropertyName);
// bool bNeedsToBePure = BindableProperty ? true : false;
// FDelegateProperty* DelegateProperty = BindableProperty ? BindableProperty : EventProperty;
