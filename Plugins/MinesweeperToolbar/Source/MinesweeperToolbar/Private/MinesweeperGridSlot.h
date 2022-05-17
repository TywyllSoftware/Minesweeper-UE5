//
//  MinesweeperGridSlot.h
//  Minesweeper_Index
//
//  Created by Tomas Mikalauskas on 14/05/2022.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//
#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Types/ISlateMetaData.h"

DECLARE_DELEGATE_TwoParams(FOnMinesweeperGridSlotSelected, int32 x, int32 y);

enum class EMinesweeperGridSlotType : int32
{
    Bomb,
    Normal,
};

class FSMinesweeperGridSlotMetadata : public ISlateMetaData
{
public:
    SLATE_METADATA_TYPE(FSMinesweeperGridSlotMetadata, ISlateMetaData)

    FSMinesweeperGridSlotMetadata(const int32 InColumn, const int32 InRow)
        :Column(InColumn), Row(InRow)
    {
    }
    
    int32 Column;
    int32 Row;
};

class SMinesweeperGridSlot final : public SCompoundWidget
{
    
public:
    
    // ------------------------------------------------------------------------
    // Public class functions
    // ------------------------------------------------------------------------
    
    SLATE_BEGIN_ARGS(SMinesweeperGridSlot) {}
        
        //Called when button is clicked
        SLATE_EVENT(FOnMinesweeperGridSlotSelected, OnGridSlotSelected)
    
    SLATE_END_ARGS()

    virtual ~SMinesweeperGridSlot() = default;
    
    void Construct(const FArguments& InArgs);
        
    void SetExpanded();
    
    bool IsExpanded();
        
    /*
        @param Type
     */
    void SetSlotType(const EMinesweeperGridSlotType Type);
    
    EMinesweeperGridSlotType GetSlotType() const;
    
    void SetSlotValue(const int32 InValue);
    
    void SetSlotTextBasedOnCurrentType();
    
protected:
    
    // ------------------------------------------------------------------------
    // Protected class functions
    // ------------------------------------------------------------------------
    
    /*
        @param Number
        @return
     */
    static FLinearColor GetNumberColor(const int32 Number);
    
    void SetFlagged();
    
    bool IsFlagged() const;
    
protected:
    
    // ------------------------------------------------------------------------
    // Protected class functions (Callbacks)
    // ------------------------------------------------------------------------

    FReply OnSlotClicked();
    
private:
    
    TSharedPtr<STextBlock> m_ButtonText;
    EMinesweeperGridSlotType m_SlotType = EMinesweeperGridSlotType::Normal;
    bool m_bIsFlagged = false;
    bool m_bIsExpanded = false;
    
    FOnMinesweeperGridSlotSelected m_OnGridSlotSelected;
    
    /** Metadata associated with this widget. */
    //TSharedRef<ISlateMetaData> m_MetaData;
};
