// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockFunctionDropdownWidget.h"

#include "Widgets/Layout/SScrollBox.h"

//-----------------------------------------------------------------------------
// SRockFunctionDropdownWidget implementation
//-----------------------------------------------------------------------------
void SRockFunctionDropdownWidget::Construct(const FArguments& InArgs)
{
    FunctionSelectedCallback = InArgs._OnFunctionSelected;
    MaxListHeight = InArgs._MaxListHeight;
    
    FunctionReferenceHandle = InArgs._EventFunctionReferenceHandle;
    
    if (InArgs._AvailableFunctions.Num() > 0)
    {
        AvailableFunctions = InArgs._AvailableFunctions;
    }
    
    ComboButton = SNew(SComboButton)
        .IsEnabled(AvailableFunctions.Num() > 0)
        // .ComboButtonStyle(InArgs._ComboButtonStyle)
        //.ButtonStyle(InArgs._ButtonStyle)
        .ButtonContent()
        [
            //InArgs._ButtonContent.Widget
            SNew(STextBlock)
            .Text(this, &SRockFunctionDropdownWidget::GetFNameFromPropertyHandle, FunctionReferenceHandle)
            
        ]
        .OnGetMenuContent(this, &SRockFunctionDropdownWidget::CreateDropdownMenu)
        .ContentPadding(InArgs._ContentPadding);
    // Set the root widget
    ChildSlot
    [
        ComboButton.ToSharedRef()
    ];
}

FText SRockFunctionDropdownWidget::GetFNameFromPropertyHandle(TSharedPtr<IPropertyHandle> PropertyHandle) const
{
    if (AvailableFunctions.IsEmpty())
    {
        return FText::FromString("None");
    }
    void* StructData = nullptr;
    if (PropertyHandle->GetValueData(StructData) == FPropertyAccess::Success)
    {
        check(StructData);
        const FName Name = *static_cast<FName*>(StructData);
        return FText::FromName(Name);
    }
    return FText::FromString("");
}

TSharedRef<SWidget> SRockFunctionDropdownWidget::CreateDropdownMenu()
{
    const bool bShouldCloseWindowAfterMenuSelection = true;
    FMenuBuilder MenuBuilder(bShouldCloseWindowAfterMenuSelection, nullptr);
    
    BuildFunctionsMenu(MenuBuilder);
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

void SRockFunctionDropdownWidget::BuildFunctionsMenu(FMenuBuilder& MenuBuilder)
{
    // Sort functions by class for organization
    TMap<UClass*, TArray<UFunction*>> FunctionsByClass;
    
    for (UFunction* Function : AvailableFunctions)
    {
        if (Function && Function->GetOwnerClass())
        {
            UClass* FunctionClass = Function->GetOwnerClass();
            
            if (!FunctionsByClass.Contains(FunctionClass))
            {
                FunctionsByClass.Add(FunctionClass, TArray<UFunction*>());
            }
            
            FunctionsByClass[FunctionClass].Add(Function);
        }
    }
    
    // Add each class as a section
    for (auto& ClassFunctionPair : FunctionsByClass)
    {
        UClass* FunctionClass = ClassFunctionPair.Key;
        TArray<UFunction*>& ClassFunctions = ClassFunctionPair.Value;
        
        // Skip empty classes
        if (ClassFunctions.Num() == 0)
        {
            continue;
        }
        
        TSharedRef<SHorizontalBox> EmptyFunctionBox = SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .HAlign(HAlign_Left)
                .Padding(FMargin(0, 0, 20, 0))
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("None"))
                ]
                + SHorizontalBox::Slot()
                .VAlign(VAlign_Center)
                .HAlign(HAlign_Right)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(""))
                    .ColorAndOpacity(FSlateColor::UseSubduedForeground())
                ];
        UFunction* emptyFunction = nullptr;
        MenuBuilder.AddMenuEntry(
                FUIAction(
                    FExecuteAction::CreateSP(
                        this,
                        &SRockFunctionDropdownWidget::HandleFunctionSelection,
                        emptyFunction,
                        ESelectInfo::OnMouseClick)
                ),
                EmptyFunctionBox,
                NAME_None,
                FText::GetEmpty(),
                EUserInterfaceActionType::Button
            );
        
        
        // Add section for this class
        FString ClassName = FunctionClass->GetName();
        MenuBuilder.BeginSection(NAME_None, FText::FromString(ClassName));
        
        // Add each function as a menu entry
        for (UFunction* Function : ClassFunctions)
        {
            // Make a horizontal box with the name on the left aligned, and the defining class on the right aligned
            TSharedRef<SHorizontalBox> FunctionBox = SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .HAlign(HAlign_Left)
                .Padding(FMargin(0, 0, 20, 0))
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(Function->GetName()))
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
                        &SRockFunctionDropdownWidget::HandleFunctionSelection,
                        Function,
                        ESelectInfo::OnMouseClick)
                ),
                FunctionBox,
                NAME_None,
                FText::GetEmpty(),
                EUserInterfaceActionType::Button
            );
        }
        
        MenuBuilder.EndSection();
    }
}

void SRockFunctionDropdownWidget::HandleFunctionSelection(UFunction* Function, ESelectInfo::Type SelectType)
{
    // Close the combo button
    ComboButton->SetIsOpen(false);
    
    // Forward the selection to the callback
    if (FunctionSelectedCallback.IsBound())
    {
        FunctionSelectedCallback.Execute(Function, SelectType);
    }
    //FunctionReferenceHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
    //FunctionReferenceHandle->NotifyFinishedChangingProperties();
    //ComboButton->Invalidate(EInvalidateWidget::Layout);
    
}


void SRockFunctionDropdownWidget::SetAvailableFunctions(const TArray<UFunction*>& InFunctions)
{
    AvailableFunctions = InFunctions;
}
