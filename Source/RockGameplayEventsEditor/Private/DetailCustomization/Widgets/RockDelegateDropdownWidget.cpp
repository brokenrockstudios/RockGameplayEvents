// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockDelegateDropdownWidget.h"
#include "Delegate/DelegateFunctionInfo.h"
#include "Misc/MiscHelperFunctions.h"

//-----------------------------------------------------------------------------
// SRockDelegateDropdownWidget implementation
//-----------------------------------------------------------------------------
void SRockDelegateDropdownWidget::Construct(const FArguments& InArgs)
{
	DelegateSelectedCallback = InArgs._OnDelegateSelected;
	// FunctionSelectedCallback = InArgs._OnFunctionSelected;
	MaxListHeight = InArgs._MaxListHeight;

	// Set available delegates/functions if provided
	if (InArgs._AvailableDelegates.Num() > 0)
	{
		AvailableDelegates = InArgs._AvailableDelegates;
	}
	else if (InArgs._FilterByClass)
	{
		GenerateAvailableDelegatesFromClass(InArgs._FilterByClass);
	}

	ComboButton = SNew(SComboButton)
		// .ComboButtonStyle(InArgs._ComboButtonStyle)
		//.ButtonStyle(InArgs._ButtonStyle)
		.ButtonContent()
		[
			InArgs
			._ButtonContent
			.Widget
		]
		.OnGetMenuContent(this, &SRockDelegateDropdownWidget::CreateDropdownMenu)
		.ContentPadding(InArgs._ContentPadding);
	// Set the root widget
	ChildSlot
	[
		ComboButton.ToSharedRef()
	];
}

TSharedRef<SWidget> SRockDelegateDropdownWidget::CreateDropdownMenu()
{
	const bool bShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder MenuBuilder(bShouldCloseWindowAfterMenuSelection, nullptr);

	BuildDelegatesMenu(MenuBuilder);

	// Wrap the menu in a scrollbox with max height
	return SNew(SBox)
		.MaxDesiredHeight(MaxListHeight)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				MenuBuilder.MakeWidget()
			]
		];
}

void SRockDelegateDropdownWidget::BuildDelegatesMenu(FMenuBuilder& MenuBuilder)
{
	// Sort delegates by class for organization
	TMap<UClass*, TArray<TSharedPtr<FRockDelegateInfo>>> DelegatesByClass;

	for (TSharedPtr<FRockDelegateInfo> Delegate : AvailableDelegates)
	{
		if (Delegate->DefiningClass)
		{
			if (!DelegatesByClass.Contains(Delegate->DefiningClass))
			{
				DelegatesByClass.Add(Delegate->DefiningClass, TArray<TSharedPtr<FRockDelegateInfo>>());
			}

			DelegatesByClass[Delegate->DefiningClass].Add(Delegate);
		}
	}

	// Add each class as a section
	for (auto& ClassDelegatePair : DelegatesByClass)
	{
		UClass* DelegateClass = ClassDelegatePair.Key;
		TArray<TSharedPtr<FRockDelegateInfo>>& ClassDelegates = ClassDelegatePair.Value;

		// Skip empty classes
		if (ClassDelegates.Num() == 0)
		{
			continue;
		}

		// Add section for this class
		FString ClassName = DelegateClass->GetName();
		MenuBuilder.BeginSection(NAME_None, FText::FromString(ClassName));

		// Add each delegate as a menu entry
		for (TSharedPtr<FRockDelegateInfo> Delegate : ClassDelegates)
		{
			// Make a horizontal box with the name on the left aligned, and the defining class on the right aligned
			TSharedRef<SHorizontalBox> DelegateBox = SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.Padding(FMargin(0, 0, 20, 0))
				[
					SNew(STextBlock)
					.Text(FText::FromString(Delegate->GetName() + UMiscHelperFunctions::BuildFunctionParameterString(Delegate->SignatureFunction)))
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Right)
				[
					SNew(STextBlock)
					.Text(FText::FromString(ClassName))
					.ColorAndOpacity(FSlateColor::UseSubduedForeground())
				];

			MenuBuilder.AddMenuEntry(
				FUIAction(
					FExecuteAction::CreateSP(
						this,
						&SRockDelegateDropdownWidget::HandleDelegateSelection,
						Delegate,
						ESelectInfo::OnMouseClick)
				),
				DelegateBox,
				NAME_None,
				FText::GetEmpty(),
				EUserInterfaceActionType::Button
			);
		}

		MenuBuilder.EndSection();
	}
}

void SRockDelegateDropdownWidget::HandleDelegateSelection(TSharedPtr<FRockDelegateInfo> Delegate, ESelectInfo::Type SelectType)
{
	// Close the combo button
	ComboButton->SetIsOpen(false);

	// Forward the selection to the callback
	if (DelegateSelectedCallback.IsBound())
	{
		DelegateSelectedCallback.Execute(Delegate, SelectType);
	}
}

void SRockDelegateDropdownWidget::SetAvailableDelegates(const TArray<TSharedPtr<FRockDelegateInfo>>& InDelegates)
{
	AvailableDelegates = InDelegates;
}

void SRockDelegateDropdownWidget::GenerateAvailableDelegatesFromClass(UClass* FilterClass)
{
	// Clear existing delegates
	AvailableDelegates.Empty();

	if (!FilterClass)
	{
		return;
	}

	// This is a simplified implementation - in a real implementation, you would need to:
	// 1. Iterate through all properties of the class and its parent classes
	// 2. Look for multicast delegate properties
	// 3. Create FRockDelegateInfo objects for each

	// Example implementation (this won't work directly as is - just illustrating the concept)
	for (TFieldIterator<FMulticastDelegateProperty> PropertyIt(FilterClass); PropertyIt; ++PropertyIt)
	{
		FMulticastDelegateProperty* DelegateProperty = *PropertyIt;

		if (DelegateProperty)
		{
			TSharedPtr<FRockDelegateInfo> DelegateInfo = MakeShared<FRockDelegateInfo>();
			DelegateInfo->DefiningClass = DelegateProperty->GetOwnerClass();
			// TODO: Prefer FName or StringName?
			FName DelegateInfoFName = DelegateProperty->GetFName();
			DelegateInfo->Name = DelegateProperty->GetName();
			DelegateInfo->SignatureFunction = DelegateProperty->SignatureFunction;

			AvailableDelegates.Add(DelegateInfo);
		}
	}
}
