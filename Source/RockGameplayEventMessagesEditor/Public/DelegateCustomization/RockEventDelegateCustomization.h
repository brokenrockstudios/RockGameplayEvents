// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

class FRockEventDelegateDetails : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	
private:
	// ~Begin IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& InStructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InStructCustomizationUtils) override {};
	// ~End IPropertyTypeCustomization interface
	
	void UpdateFunctionList();

	FText GetSelectedFunctionName() const;
	TSharedRef<SWidget> MakeFunctionItemWidget(const TSharedPtr<FString> InItem);
	void OnFunctionSelected(const TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
	bool IsCompatibleFunction(const UFunction* TestFunction) const;
	void OnTargetActorChanged(const FAssetData& NewAssetData);
	
	
private:
	TArray<TSharedPtr<FString>> FunctionList;
	TSharedPtr<IPropertyHandle> TargetActorHandle;
	TSharedPtr<IPropertyHandle> EventFunctionReferenceHandle;
	UFunction* PrototypeFunction = nullptr;
};
