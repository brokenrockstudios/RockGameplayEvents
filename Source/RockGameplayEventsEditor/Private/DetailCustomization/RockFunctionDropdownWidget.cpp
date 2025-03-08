// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailCustomization/RockFunctionDropdownWidget.h"

//-----------------------------------------------------------------------------
// SRockFunctionDropdownWidget implementation
//-----------------------------------------------------------------------------
void SRockFunctionDropdownWidget::Construct(const FArguments& InArgs)
{
    FunctionSelectedCallback = InArgs._OnFunctionSelected;
    MaxListHeight = InArgs._MaxListHeight;
    
    if (InArgs._AvailableFunctions.Num() > 0)
    {
        AvailableFunctions = InArgs._AvailableFunctions;
    }
    else if (InArgs._FilterByClass)
    {
        // GenerateAvailableFunctionsFromClass(InArgs._FilterByClass);
    }
    
    
    ComboButton = SNew(SComboButton)
        // .ComboButtonStyle(InArgs._ComboButtonStyle)
        //.ButtonStyle(InArgs._ButtonStyle)
        .ButtonContent()
        [
            InArgs._ButtonContent.Widget
        ]
        .OnGetMenuContent(this, &SRockFunctionDropdownWidget::CreateDropdownMenu)
        .IsEnabled(AvailableFunctions.Num() > 0)
        .ContentPadding(InArgs._ContentPadding);
    // Set the root widget
    ChildSlot
    [
        ComboButton.ToSharedRef()
    ];
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
}


void SRockFunctionDropdownWidget::SetAvailableFunctions(const TArray<UFunction*>& InFunctions)
{
    AvailableFunctions = InFunctions;
}


void SRockFunctionDropdownWidget::GenerateAvailableFunctionsFromClass(UClass* FilterClass)
{
    // Clear existing functions
    AvailableFunctions.Empty();
    
    if (!FilterClass)
    {
        return;
    }
    
    // Iterate through functions in the class and its parent classes
    for (TFieldIterator<UFunction> FunctionIt(FilterClass, EFieldIteratorFlags::IncludeSuper); FunctionIt; ++FunctionIt)
    {
        UFunction* Function = *FunctionIt;
        
        // Add filtering logic here if needed
        // For example, only include BlueprintCallable functions
        if (Function && Function->HasAnyFunctionFlags(FUNC_BlueprintCallable))
        {
            AvailableFunctions.Add(Function);
        }
    }
}