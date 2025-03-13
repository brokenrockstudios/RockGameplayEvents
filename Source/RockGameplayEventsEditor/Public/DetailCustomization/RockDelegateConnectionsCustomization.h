// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Delegate/RockGameplayEventConnection.h"
#include "UObject/Object.h"

struct FRockDelegateInfo;
class URockDelegateConnectorComponent;

class FRockDelegateConnectionsCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// ~Begin IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& InStructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InStructCustomizationUtils) override;
	// ~End IPropertyTypeCustomization interface
	
	// ~Begin Helper Functions
	void UpdateFunctionList(const TSharedPtr<IPropertyHandle>& TargetActorHandle);
	void OnTargetActorSelected(const FAssetData& asset_data, TSharedPtr<IPropertyHandle> ElementHandle);

	void OnMulticastDelegateSelected(TSharedPtr<FRockDelegateInfo> InItem, ESelectInfo::Type arg);
	void OnFunctionSelected(const UFunction* SelectedFunction, ESelectInfo::Type someType, TSharedPtr<IPropertyHandle> ElementHandle)  const;
	
	FText GetSelectedDelegate() const;
	FText GetDelegateParameterList() const;

	AActor* GetActorFromHandle(const TSharedPtr<IPropertyHandle>& PropertyHandle) const;
	// ~End Helper Functions



	void CacheData();
	// The root should be the FRockGameplayEventConnection struct
	TSharedPtr<IPropertyHandle> MyPropertyHandle;
	TSharedPtr<IPropertyHandle> BindingsHandler;
	TSharedPtr<IPropertyHandle> DelegatePropertyNameHandler;
	TSharedPtr<IPropertyHandle> DelegateParameterListHandler;
	TSharedPtr<IPropertyHandle> DelegateTypeHandler;

	FRockGameplayEventConnection* CachedConnection;
	TWeakObjectPtr<UObject> CachedOuterObject;
	TWeakObjectPtr<AActor> CachedActor;
	TWeakObjectPtr<URockDelegateConnectorComponent> CachedActorComponent;
	
	UClass* CachedActorClass;
	
	// Cached actor class
	
	// TSharedPtr<IPropertyHandle> MyStructProperty;
	// TSharedPtr<IPropertyHandle> RootBindingHandle;
	// BindingsHandle should be FRockGameplayEventConnection::Bindings
	
	TArray<TSharedPtr<FRockDelegateInfo>> AvailableDelegates;
	TMap<TSharedPtr<IPropertyHandle>, TArray<UFunction*>> ElementFunctionListMap;

	// TSharedPtr<FRockDelegateInfo> SelectedDelegate;
	
	TSharedPtr<STextBlock> DelegateParamListRef;
	TSharedPtr<SVerticalBox> Vert;
	
	TAttribute<float> SplitterCoefficient;
	IPropertyUtilities* PropUtils;
};
