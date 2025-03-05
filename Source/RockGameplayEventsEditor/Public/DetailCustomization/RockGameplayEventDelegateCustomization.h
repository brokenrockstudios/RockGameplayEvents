// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

class FRockGameplayEventDelegateCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// ~Begin IPropertyTypeCustomization interface
	virtual void CustomizeHeader(
		TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& InStructCustomizationUtils) override;
	virtual void CustomizeChildren(
		TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& InChildBuilder,
		IPropertyTypeCustomizationUtils& InStructCustomizationUtils) override;
	// ~End IPropertyTypeCustomization interface

private:
	// ~Begin Helper Functions
	FText GetSelectedFunctionName(TSharedPtr<IPropertyHandle> PropertyHandle) const;
	void UpdateFunctionList(const TSharedPtr<IPropertyHandle>& TargetActorHandle);
	bool IsCompatibleFunction(const UFunction* TestFunction) const;
	void OnFunctionChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo, TSharedPtr<IPropertyHandle> ElementHandle) const;
	AActor* GetActorFromHandle(const TSharedPtr<IPropertyHandle>& PropertyHandle) const;
	FText GetArrayTextValue(uint32 NumElements) const;
	UBlueprint* GetBlueprintClassFromActor(const AActor* Actor) const;
	// ~End Helper Functions

private:
	// I don't care for this approach? :( 
	TMap<TSharedPtr<IPropertyHandle>, TArray<TSharedPtr<FString>>> ElementFunctionListMap;
	
	// We use the same prototype function for all functions at this time. So we can cache it
	UFunction* PrototypeFunction = nullptr;
};
