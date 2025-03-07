// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Component/RockDelegateConnectorComponent.h"
#include "UObject/Object.h"

struct FRockDelegateInfo;
class URockDelegateConnectorComponent;

class FRockGameplayEventDelegateConnectionsCustomization : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// ~Begin IPropertyTypeCustomization interface
	virtual void CustomizeHeader(
		TSharedRef<IPropertyHandle> PropertyHandle,
		FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& InStructCustomizationUtils) override;

	FReply ToggleDropDown();
	virtual void CustomizeChildren(
		TSharedRef<IPropertyHandle> PropertyHandle,
		IDetailChildrenBuilder& InChildBuilder,
		IPropertyTypeCustomizationUtils& InStructCustomizationUtils) override;
	// ~End IPropertyTypeCustomization interface

	// ~Begin Helper Functions
	FText GetSelectedFunctionName(TSharedPtr<IPropertyHandle> PropertyHandle) const;
	UBlueprint* GetBlueprintClassFromActor(const AActor* Actor) const;
	AActor* GetActorFromHandle(const TSharedPtr<IPropertyHandle>& PropertyHandle) const;
	// Need to incorporate which Event?
	bool IsCompatibleFunction(const UFunction* TestFunction) const;
	void OnFunctionChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo, TSharedPtr<IPropertyHandle> ElementHandle) const;
	void UpdateFunctionList(const TSharedPtr<IPropertyHandle>& TargetActorHandle);
	const UClass* GetPropertyOwnerClassFromBindingsHandler();


	TSharedRef<SWidget> MakePopupButton();
	TSharedRef<SWidget> CreateAddNewPopupSearch();
	void BuildOverridableFunctionsMenu(FMenuBuilder& MenuBuilder);

	

	TSharedRef<SWidget> OnGetMenuContent();
	void OnMulticastDelegateSelected(TSharedPtr<FRockDelegateInfo> InItem, ESelectInfo::Type arg);


	
	void PrintHelloWorld() const;
	// ~End Helper Functions
	
	FText GetSelectedDelegate() const;

	TSharedPtr<IPropertyHandle> BindingsHandler;
	TSharedPtr<IPropertyHandle> DelegatePropertyNameHandler;
	TSharedPtr<IPropertyHandle> DelegatePropertyTypeHandler;
	TMap<TSharedPtr<IPropertyHandle>, TArray<TSharedPtr<FString>>> ElementFunctionListMap;
	//FunctionList 
	TSharedPtr<SMenuAnchor> MenuAnchor;

	

	TArray<TSharedPtr<FRockDelegateInfo>> AvailableDelegates;
	TArray<TSharedPtr<FRockDelegateInfo>>  FilterableDelegates;
	TArray<TSharedPtr<FString>> AvailableDelegatesStrings;
	
	//TArray<TSharedPtr<FName>> AvailableMulticastDelegatesNames;
	//TMap<FName, FMulticastDelegateProperty*> AvailableMulticastDelegates;
	TSharedPtr<FRockDelegateInfo> SelectedDelegate;
	// FMulticastDelegateProperty* SelectedMulticastDelegate = nullptr;
	
	
	TSharedPtr<UFunction> PrototypeFunction;
	
	TAttribute<float> SplitterCoefficient;
};
