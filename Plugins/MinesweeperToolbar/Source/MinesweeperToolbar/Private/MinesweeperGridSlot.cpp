//
//  MinesweeperGridSlot.cpp
//  Minesweeper_Index
//
//  Created by Tomas Mikalauskas on 14/05/2022.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#include "MinesweeperGridSlot.h"

//Engine includes
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"

//Editor includes
#include "UtilGridButtonStyle.h"

#define LOCTEXT_NAMESPACE "FMinesweeperToolbarModule"

// ------------------------------------------------------------------------
// Public class functions
// ------------------------------------------------------------------------

void SMinesweeperGridSlot::SMinesweeperGridSlot::Construct(const FArguments& InArgs)
{
    m_OnGridSlotSelected = InArgs._OnGridSlotSelected;
    
    ChildSlot
    [
        SNew(SButton)
        .ButtonStyle(FUtilGridButtonStyle::Get(), "MinesweeperGridButton.Button")
        .OnClicked(this, &SMinesweeperGridSlot::OnSlotClicked)
        .IsEnabled_Lambda([this]
        {
            return !IsExpanded();
        })
        [
            SAssignNew(m_ButtonText, STextBlock)
            .Justification(ETextJustify::Center)
            .ShadowOffset(1.f)
            .HighlightColor(FColor::Red)
            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
            .Text_Lambda([this]
            {
                FFormatNamedArguments Args;
                Args.Add(TEXT("Flag"), FText::FromString(m_bIsFlagged ? TEXT("F") : TEXT("")));
                return FText::Format(LOCTEXT("FlagText", "{Flag}"), Args);
            })
        ]
    ];
}

void SMinesweeperGridSlot::SetExpanded()
{
    m_bIsExpanded = true;
}

bool SMinesweeperGridSlot::IsExpanded()
{
    return m_bIsExpanded;
}

void SMinesweeperGridSlot::SetSlotType(const EMinesweeperGridSlotType Type)
{
    m_SlotType = Type;
}

EMinesweeperGridSlotType SMinesweeperGridSlot::GetSlotType() const
{
    return m_SlotType;
}

void SMinesweeperGridSlot::SetSlotValue(const int32 InValue)
{
    m_ButtonText->SetText(FText::AsNumber(InValue));
    m_ButtonText->SetColorAndOpacity(GetNumberColor(InValue));
}

void SMinesweeperGridSlot::SetSlotTextBasedOnCurrentType()
{
    switch(m_SlotType)
    {
        case EMinesweeperGridSlotType::Bomb:
            m_ButtonText->SetText(LOCTEXT("BombText", "B"));
            break;
        case EMinesweeperGridSlotType::Normal:
            //nothing
            break;
        default:
            break;
    }
}

// ------------------------------------------------------------------------
// Protected class functions
// ------------------------------------------------------------------------

FLinearColor SMinesweeperGridSlot::GetNumberColor(const int32 Number)
{
    switch (Number)
    {
        case 1: return FColor::Green;
        case 2: return FColor::Orange;
        default: return FColor::Red;
    }
}

void SMinesweeperGridSlot::SetFlagged()
{
    m_bIsFlagged = true;
}

bool SMinesweeperGridSlot::IsFlagged() const
{
    return m_bIsFlagged;
}

// ------------------------------------------------------------------------
// Protected class functions (Callbacks)
// ------------------------------------------------------------------------

FReply SMinesweeperGridSlot::SMinesweeperGridSlot::OnSlotClicked()
{
    //Don't do anything if we already clicked on it or if it was expanded
    if(IsFlagged() || IsExpanded())
    {
        return FReply::Handled();
    }
    
    //Flag button
    SetFlagged();
    
    TSharedPtr<FSMinesweeperGridSlotMetadata> WidgetMetaData = GetMetaData<FSMinesweeperGridSlotMetadata>();
    check(WidgetMetaData.IsValid());
    
    SetSlotTextBasedOnCurrentType();
    m_OnGridSlotSelected.ExecuteIfBound(WidgetMetaData->Column, WidgetMetaData->Row);
    
    return FReply::Handled();
}


#undef LOCTEXT_NAMESPACE
