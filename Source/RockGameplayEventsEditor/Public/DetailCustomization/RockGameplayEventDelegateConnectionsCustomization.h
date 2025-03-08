// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

struct FRockDelegateInfo;
class URockDelegateConnectorComponent;

class FRockGameplayEventDelegateConnectionsCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// ~Begin IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& InStructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InStructCustomizationUtils) override;
	// ~End IPropertyTypeCustomization interface
	
	// ~Begin Helper Functions
	void UpdateFunctionList(const TSharedPtr<IPropertyHandle>& TargetActorHandle);

	void OnMulticastDelegateSelected(TSharedPtr<FRockDelegateInfo> InItem, ESelectInfo::Type arg);
	void OnFunctionSelected(UFunction* theFunction, ESelectInfo::Type someType, TSharedPtr<IPropertyHandle> ElementHandle);
	
	FText GetSelectedDelegate() const;
	FText GetSelectedFunctionName(TSharedPtr<IPropertyHandle> PropertyHandle) const;

	AActor* GetActorFromHandle(const TSharedPtr<IPropertyHandle>& PropertyHandle) const;
	const UClass* GetPropertyOwnerClassFromBindingsHandler() const;
	// ~End Helper Functions

	TSharedPtr<IPropertyHandle> BindingsHandler;
	TSharedPtr<IPropertyHandle> DelegatePropertyNameHandler;
	TSharedPtr<IPropertyHandle> DelegatePropertyTypeHandler;
	
	TArray<TSharedPtr<FRockDelegateInfo>> AvailableDelegates;
	TMap<TSharedPtr<IPropertyHandle>, TArray<UFunction*>> ElementFunctionListMap;

	TSharedPtr<FRockDelegateInfo> SelectedDelegate;
	
	TAttribute<float> SplitterCoefficient;
};
