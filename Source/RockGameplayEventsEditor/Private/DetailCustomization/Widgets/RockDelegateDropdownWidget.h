// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

struct FRockDelegateInfo;
DECLARE_DELEGATE_TwoParams(FOnDelegateSelected, TSharedPtr<FRockDelegateInfo>, ESelectInfo::Type);

class ROCKGAMEPLAYEVENTSEDITOR_API SRockDelegateDropdownWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SRockDelegateDropdownWidget)
		: _ComboButtonStyle(nullptr)
		, _ButtonStyle(nullptr)
		, _ContentPadding(FMargin(4.0f, 2.0f))
		, _MaxListHeight(450.0f)
		, _ButtonContent()
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
	SLATE_DEFAULT_SLOT(FArguments, ButtonContent)
	/** Callback for when a delegate is selected */
	SLATE_EVENT(FOnDelegateSelected, OnDelegateSelected)
	/** Optional list of available delegates to show (will be generated if not provided) */
	SLATE_ARGUMENT(TArray<TSharedPtr<FRockDelegateInfo>>, AvailableDelegates)
SLATE_END_ARGS()

	/** Constructs this widget */
	void Construct(const FArguments& InArgs);
	/** Sets the list of available delegates */
	void SetAvailableDelegates(const TArray<TSharedPtr<FRockDelegateInfo>>& InDelegates);
private:
	/** Creates the dropdown menu widget */
	TSharedRef<SWidget> CreateDropdownMenu();
	/** Builds the menu for delegates */
	void BuildDelegatesMenu(FMenuBuilder& MenuBuilder);
	/** Handler for delegate selection */
	void HandleDelegateSelection(TSharedPtr<FRockDelegateInfo> Delegate, ESelectInfo::Type SelectType);
    
private:
	/** Callback for delegate selection */
	FOnDelegateSelected DelegateSelectedCallback;
    
	/** List of available delegates */
	TArray<TSharedPtr<FRockDelegateInfo>> AvailableDelegates;
    
	/** The combo button widget */
	TSharedPtr<SComboButton> ComboButton;
    
	/** Max height of the dropdown list */
	float MaxListHeight = 450.0f;
};
