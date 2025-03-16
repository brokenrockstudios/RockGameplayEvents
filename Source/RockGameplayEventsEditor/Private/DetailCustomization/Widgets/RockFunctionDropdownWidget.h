// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

struct FRockDelegateInfo;
DECLARE_DELEGATE_TwoParams(FOnFunctionSelected, const UFunction*, ESelectInfo::Type);

class ROCKGAMEPLAYEVENTSEDITOR_API SRockFunctionDropdownWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SRockFunctionDropdownWidget)
		: _ComboButtonStyle(nullptr)
		, _ButtonStyle(nullptr)
		, _ContentPadding(FMargin(4.0f, 2.0f))
		, _MaxListHeight(450.0f)
		//, _ButtonContent()
	{}
	/** The visual style of the combo button */
	SLATE_ARGUMENT(const FComboButtonStyle*, ComboButtonStyle)
	/** The visual style of the button */
	SLATE_ARGUMENT(const FButtonStyle*, ButtonStyle)
	/** Sets the padding for the button's content area */
	SLATE_ARGUMENT(FMargin, ContentPadding)
	/** The max height of the dropdown list */
	SLATE_ARGUMENT(float, MaxListHeight)
	/** The content to be displayed in the button */
	// Was running into validation/redrawing issues, so instead just passing in the IPropertyHandle here
	// SLATE_DEFAULT_SLOT(FArguments, ButtonContent)
	/** Callback for when a delegate is selected */
	SLATE_EVENT(FOnFunctionSelected, OnFunctionSelected)
	/** Optional list of available functions to show (will be generated if not provided) */
	SLATE_ARGUMENT(TArray<UFunction*>, AvailableFunctions)
	//void EventFunctionReferenceHandle(bool shared);
	SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, EventFunctionReferenceHandle)
SLATE_END_ARGS()

	/** Constructs this widget */
	void Construct(const FArguments& InArgs);

	/** Sets the list of available functions */
	void SetAvailableFunctions(const TArray<UFunction*>& InFunctions);
	
private:
	/** Creates the dropdown menu widget */
	TSharedRef<SWidget> CreateDropdownMenu();
	TSharedPtr<IPropertyHandle> FunctionReferenceHandle;
	
	/** Builds the menu for functions */
	void BuildFunctionsMenu(FMenuBuilder& MenuBuilder);

	/** Handler for function selection */
	void HandleFunctionSelection(UFunction* Function, ESelectInfo::Type SelectType);

	/** Generates a list of available functions from the filter class */
	void GenerateAvailableFunctionsFromClass(UClass* FilterClass);

	
	FText GetFNameFromPropertyHandle(TSharedPtr<IPropertyHandle> PropertyHandle) const;
private:
	/** Whether to show delegates or functions */
	bool bShowDelegates = true;
    
	/** Callback for function selection */
	FOnFunctionSelected FunctionSelectedCallback;
    
	/** List of available functions */
	TArray<UFunction*> AvailableFunctions;
    
	/** The combo button widget */
	TSharedPtr<SComboButton> ComboButton;
    
	/** Max height of the dropdown list */
	float MaxListHeight = 450.0f;
};


