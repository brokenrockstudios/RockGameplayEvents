// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "DetailCustomization/RockGameplayEventDelegateConnectionsCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "RandomFunctions.h"
#include "Component/RockDelegateConnectorComponent.h"
#include "DetailCustomization/RockDelegateDropdownWidget.h"
#include "DetailCustomization/RockFunctionDropdownWidget.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Misc/MiscHelperFunctions.h"
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

	// BindingsHandler->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(
	// 	[&InStructCustomizationUtils]()
	// 	{
	// 		//InStructCustomizationUtils.GetPropertyUtilities()->ForceRefresh();
	// 	}));

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
	AvailableDelegatesStrings.Empty();
	auto delegates = UMiscHelperFunctions::GetDelegatesForActorClass(PropertyOwnerClass);

	for (const FRockDelegateInfo& Delegate : delegates)
	{
		AvailableDelegates.Add(MakeShareable(new FRockDelegateInfo(Delegate)));
	}
	FilterableDelegates = AvailableDelegates;

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
		//	MakePopupButton()
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
						// {
						// 	UE_LOG(LogTemp, Warning, TEXT("Function Selected"));
						// 	//OnFunctionChanged(Function, SelectType, ElementHandle);
						// })
						
						
					// SNew(SComboBox<TSharedPtr<FString>>)
					// .OptionsSource(&FunctionList)
					// .OnGenerateWidget_Lambda([](const TSharedPtr<FString>& InItem)
					// {
					// 	return SNew(STextBlock).Text(FText::FromString(*InItem));
					// })
					// .OnSelectionChanged(this, &FRockGameplayEventDelegateConnectionsCustomization::OnFunctionChanged, ElementHandle)
					// [
					// 	SNew(STextBlock)
					// 	.Text(this, &FRockGameplayEventDelegateConnectionsCustomization::GetSelectedFunctionName, ElementHandle)
					// ]
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


FText FRockGameplayEventDelegateConnectionsCustomization::GetSelectedFunctionName(TSharedPtr<IPropertyHandle> PropertyHandle) const
{
	const TSharedPtr<IPropertyHandle> TargetActorHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, TargetActor), false);
	const TSharedPtr<IPropertyHandle> EventFunctionReferenceHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, FunctionNameToBind), false);

	const AActor* Actor = GetActorFromHandle(TargetActorHandle);
	const UBlueprint* Blueprint = GetBlueprintClassFromActor(Actor);
	if (TargetActorHandle.IsValid() && EventFunctionReferenceHandle.IsValid())
	{
		void* StructData = nullptr;
		const FPropertyAccess::Result Result = EventFunctionReferenceHandle->GetValueData(StructData);
		if (Result == FPropertyAccess::Success)
		{
			check(StructData);
			//FMemberReference* MemberReference = static_cast<FMemberReference*>(StructData);
			FName Name = *static_cast<FName*>(StructData);
			return FText::FromName(Name);
			//if (UFunction* Function = MemberReference->ResolveMember<UFunction>(Blueprint->SkeletonGeneratedClass))
			// {
			//	return FText::FromName(Function->GetFName());
			//}
		}
	}
	return FText::FromString("");
}


UBlueprint* FRockGameplayEventDelegateConnectionsCustomization::GetBlueprintClassFromActor(const AActor* Actor) const
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

AActor* FRockGameplayEventDelegateConnectionsCustomization::GetActorFromHandle(const TSharedPtr<IPropertyHandle>& PropertyHandle) const
{
	UObject* TargetActorObject = nullptr;
	PropertyHandle->GetValue(TargetActorObject);
	return Cast<AActor>(TargetActorObject);
}

void FRockGameplayEventDelegateConnectionsCustomization::OnFunctionSelected( UFunction* theFunction, ESelectInfo::Type someType, TSharedPtr<IPropertyHandle> ElementHandle)
{
	OnFunctionChanged(MakeShareable(new FString(theFunction->GetName())), someType, ElementHandle);
}
	
	
void FRockGameplayEventDelegateConnectionsCustomization::OnFunctionChanged(
	TSharedPtr<FString> Item,
	ESelectInfo::Type SelectInfo,
	TSharedPtr<IPropertyHandle> ElementHandle) const
{
	FString itemStr = *Item;
	UE_LOG(LogTemp, Warning, TEXT("OnFunctionChanged Item: %s"), *itemStr);

	const TSharedPtr<IPropertyHandle> TargetActorHandle = ElementHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, TargetActor), false);
	const TSharedPtr<IPropertyHandle> EventFunctionReferenceHandle = ElementHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FRockGameplayEventBinding, FunctionNameToBind), false);
	if (Item.IsValid() && TargetActorHandle.IsValid() && EventFunctionReferenceHandle.IsValid())
	{
		// Get the selected function name
		const FString SelectedFunctionNameStr(*Item);
		const FName SelectedFunctionName(*Item);

		// 	const AActor* Actor = GetActorFromHandle(TargetActorHandle);
		// 	if (!Actor)
		// 	{
		// 		// Failed to cast the target actor to AActor.
		// 		return;
		// 	}
		//
		// 	const UClass* ScopeClass = Actor->GetClass();
		// 	if (ScopeClass == nullptr)
		// 	{
		// 		// Failed to retrieve the class of the target actor.
		// 		return;
		// 	}
		//
		// 	const UFunction* Function = ScopeClass->FindFunctionByName(SelectedFunctionName);
		// 	if (Function == nullptr)
		// 	{
		// 		// Failed to find the function by name.
		// 		return;
		// 	}
		//
		// 	const UBlueprint* Blueprint = GetBlueprintClassFromActor(Actor);
		// 	if (Blueprint == nullptr)
		// 	{
		// 		// Failed to cast the class generated by the blueprint.
		// 		return;
		// 	}
		//
		void* StructData = nullptr;
		const FPropertyAccess::Result Result = EventFunctionReferenceHandle->GetValueData(StructData);
		if (Result == FPropertyAccess::Success)
		{
			check(StructData)
			FName* Name = static_cast<FName*>(StructData);
			//const bool bSelfContext = (Blueprint->GeneratedClass != nullptr && Blueprint->GeneratedClass->IsChildOf(ScopeClass)) ||
			//	(Blueprint->SkeletonGeneratedClass != nullptr && Blueprint->SkeletonGeneratedClass->IsChildOf(ScopeClass));

			*Name = SelectedFunctionName;
			//MemberReference->SetFromField<UFunction>(Function, bSelfContext);
		}
	}
}

//void FRockGameplayEventDelegateConnectionsCustomization::OnMulticastDelegateSelected(TSharedPtr<FRockDelegateInfo> InItem, ESelectInfo::Type arg)
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

void FRockGameplayEventDelegateConnectionsCustomization::UpdateFunctionList(const TSharedPtr<IPropertyHandle>& TargetActorHandle)
{
	ElementFunctionListMap[TargetActorHandle].Empty();
	UFunction* NoneFunction = nullptr;

	const UClass* PropertyOwnerClass = GetPropertyOwnerClassFromBindingsHandler();
	const FText SelectedDelegate2 = GetSelectedDelegate();
	if (!PropertyOwnerClass || SelectedDelegate2.IsEmpty())
	{
		ElementFunctionListMap[TargetActorHandle].Add(NoneFunction);
		return;
	}

	FMulticastDelegateProperty* MulticastDelegateProperty = FindFProperty<FMulticastDelegateProperty>(PropertyOwnerClass,
		*SelectedDelegate2.ToString());
	if (!MulticastDelegateProperty)
	{
		ElementFunctionListMap[TargetActorHandle].Add(NoneFunction);
		return;
	}
	auto pprotoTypeFunction = MulticastDelegateProperty->SignatureFunction;

	// IsCompatiableFunction Lambda
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
		UE_LOG(LogTemp, Warning, TEXT("TargetActor: %s"), *TargetActor->GetName());
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
	// Let's always have at least 1 option
	if (Function_List.Num() == 0)
	{
		Function_List.Add(NoneFunction);
	}
}

const UClass* FRockGameplayEventDelegateConnectionsCustomization::GetPropertyOwnerClassFromBindingsHandler()
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


TSharedRef<SWidget> FRockGameplayEventDelegateConnectionsCustomization::OnGetMenuContent()
{
	

	//return newMenuContent;
	
	// This works:
	return CreateAddNewPopupSearch();


	
	//
	// return SNew(SHorizontalBox)
	// 	+ SHorizontalBox::Slot()
	// 	.AutoWidth()
	// 	.VAlign(VAlign_Center)
	// 	.Padding(0.0f, 0.0f, 5.0f, 0.0f)
	// 	[
	// 		SNew(STextBlock).Text(FText::FromString("Multicast Delegate")).Font(IDetailLayoutBuilder::GetDetailFont())
	// 	]
	// 	+ SHorizontalBox::Slot()
	// 	.FillWidth(1)
	// 	[
	// 		SNew(SVerticalBox)
	// 		+ SVerticalBox::Slot()
	// 		.AutoHeight()
	// 		[
	// 			SNew(SSearchBox)
	// 			.HintText(FText::FromString("Search for Delegates"))
	// 			.OnTextChanged(this, &FRockGameplayEventDelegateConnectionsCustomization::OnSearchTextChanged)
	// 		]
	// 		+ SVerticalBox::Slot()
	// 		[
	// 	];
}

void FRockGameplayEventDelegateConnectionsCustomization::PrintHelloWorld() const
{
	UE_LOG(LogTemp, Warning, TEXT("Hello World"));
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


TSharedRef<SWidget> FRockGameplayEventDelegateConnectionsCustomization::MakePopupButton()
{
	return SAssignNew(MenuAnchor, SMenuAnchor)
	 	.Placement(MenuPlacement_BelowAnchor)
	 	.OnGetMenuContent(this, &FRockGameplayEventDelegateConnectionsCustomization::OnGetMenuContent)
	 	[
	 		SNew(SButton)
	 		.OnClicked(this, &FRockGameplayEventDelegateConnectionsCustomization::ToggleDropDown)
	 		[
	 			SNew(STextBlock)
	 			.Text(this, &FRockGameplayEventDelegateConnectionsCustomization::GetSelectedDelegate)
	 		]
	 	];
}


FReply FRockGameplayEventDelegateConnectionsCustomization::ToggleDropDown()
{
	if (MenuAnchor->IsOpen())
	{
		MenuAnchor->SetIsOpen(false);
	}
	else
	{
		MenuAnchor->SetIsOpen(true);
	}
	return FReply::Handled();
}


TSharedRef<SWidget> FRockGameplayEventDelegateConnectionsCustomization::CreateAddNewPopupSearch()
{
	constexpr bool bInShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder MenuBuilder(bInShouldCloseWindowAfterMenuSelection, nullptr);
	BuildOverridableFunctionsMenu(MenuBuilder);
	TSharedRef<SWidget> MenuWidget = MenuBuilder.MakeWidget();
	return MenuWidget;
}

void FRockGameplayEventDelegateConnectionsCustomization::BuildOverridableFunctionsMenu(FMenuBuilder& MenuBuilder)
{
	bool bFirst = true;
	UClass* DelegateClass = nullptr;
	FString DelegateName = "";
	FString DelegateSignature = "";
	for (TSharedPtr<FRockDelegateInfo> Delegate : AvailableDelegates)
	{
		if (Delegate->DefiningClass != DelegateClass)
		{
			DelegateClass = Delegate->DefiningClass;
			DelegateName = Delegate->DefiningClass->GetName();
			//DelegateSignature = Delegate->GetNameWithClass();
			if (!bFirst)
			{
				MenuBuilder.EndSection();
			}
			bFirst = false;
			MenuBuilder.BeginSection(NAME_None, FText::FromString(DelegateName));
		}

		// Make a horizontal box with the name on the left aligned, and the defining class on the right aligned
		TSharedRef<SHorizontalBox> DelegateBox = SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Left)
			.Padding(FMargin(0, 0, 20, 0))
			[
				SNew(STextBlock)
				//.Text(FText::FromString(*Delegate->Name))
				.Text(Delegate->GetSignatureFunctionString())
			]
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			[
				SNew(STextBlock)
				.Text(FText::FromString(DelegateName))
				.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			];

		MenuBuilder.AddMenuEntry(
			FUIAction(
				FExecuteAction::CreateSP(
					this,
					&FRockGameplayEventDelegateConnectionsCustomization::OnMulticastDelegateSelected,
					Delegate,
					ESelectInfo::OnMouseClick)
			),
			DelegateBox,
			NAME_None,
			FText::FromString("Tooltip"),
			EUserInterfaceActionType::Button
		);
	}
	MenuBuilder.EndSection();
}
