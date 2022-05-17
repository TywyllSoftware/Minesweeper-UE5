//
//  MinesweeperWindow.cpp
//  Minesweeper_Index
//
//  Created by Tomas Mikalauskas on 09/05/2022.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//
#include "MinesweeperWindow.h"

//Engine Includes
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

//Plugin includes
#include "MinesweeperGridSlot.h"

#define LOCTEXT_NAMESPACE "FMinesweeperToolbarModule"

//Settings
const int32 MaxWidth = 12;
const int32 MaxHeight = 12;

const FSlateFontInfo GSlateInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16);
const int32 GIconSize = 16;
const int32 GMinDesiredWidthForEntryBox = 100;

//Localisation
const FText GHeightText = LOCTEXT("Minesweeper_Height", "Height:");
const FText GWidthText = LOCTEXT("Minesweeper_Width", "Width:");
const FText GNumberOfMinesText = LOCTEXT("Minesweeper_NumberOfMines", "Number of mines:");
const FText GGenerateNewGridText = LOCTEXT("Minesweeper_GenerateNewGrid", "Generate New Grid");
const FText GLostGameText = LOCTEXT("Minesweeper_LostGame", "You have lost game.");
const FText GWonGameText  = LOCTEXT("Minesweeper_WonGame", "You have won game!");
const FText GCanNotHaveMoreMinesText = LOCTEXT("Minesweeper_MoreMines", "Minesweeper can not have more mines than cells");
const FText GWindowTitleText = LOCTEXT("Minesweeper_Window", "Minesweeper");

// ------------------------------------------------------------------------
// Public class functions
// ------------------------------------------------------------------------

void SMinesweeperWindow::Construct(const FArguments& InArgs, UTexture2D* InSourceTexture)
{
    SAssignNew(m_UniformGridPanel, SUniformGridPanel);
    
    SAssignNew(m_PanelArea, SBorder )
    .BorderImage(FEditorStyle::GetBrush("Menu.Background"))
    .ColorAndOpacity( FLinearColor::White );
    
    ChildSlot
    [
        m_PanelArea.ToSharedRef()
    ];
    
    CreatePanelArea();
}

SMinesweeperWindow::~SMinesweeperWindow()
{
}

bool SMinesweeperWindow::ShowWindow(UTexture2D* SourceTexture)
{
    TSharedRef<SWindow> MinesweeperWindow = SNew(SWindow)
        .Title(GWindowTitleText)
        .SizingRule(ESizingRule::UserSized)
        .ClientSize(FVector2D(800, 600))
        .AutoCenter(EAutoCenter::PreferredWorkArea)
        .SupportsMinimize(false);
     
    TSharedRef<SMinesweeperWindow> MinesweeperDialog = SNew(SMinesweeperWindow, SourceTexture);

    MinesweeperWindow->SetContent(MinesweeperDialog);
    TSharedPtr<SWindow> RootWindow = FGlobalTabmanager::Get()->GetRootWindow();
    if (RootWindow.IsValid())
    {
        FSlateApplication::Get().AddWindowAsNativeChild(MinesweeperWindow, RootWindow.ToSharedRef());
    }
    else
    {
        FSlateApplication::Get().AddWindow(MinesweeperWindow);
    }

    return false;
}

// ------------------------------------------------------------------------
// Protected class functions
// ------------------------------------------------------------------------

void SMinesweeperWindow::CreatePanelArea()
{
    TSharedPtr<SOverlay> ContentArea;
    m_PanelArea->SetContent(SAssignNew(ContentArea, SOverlay));

    ContentArea->ClearChildren();
    
    ContentArea->AddSlot()
    .HAlign(HAlign_Fill)
    .VAlign(VAlign_Top)
    .Padding(FMargin(0, 50))
    [
        
        SNew(SHorizontalBox)
        
        //Width box
        + SHorizontalBox::Slot()
        .Padding(FMargin(10))
        .AutoWidth()
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Top)
        [
            SNew(STextBlock)
            .Text(GWidthText)
            .Font(GSlateInfo)
        ]
    
        + SHorizontalBox::Slot()
        .Padding(FMargin(10))
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Fill)
        [
            SNew(SNumericEntryBox<int32>)
            .Value(this, &SMinesweeperWindow::GetWidth)
            .MinDesiredValueWidth(GMinDesiredWidthForEntryBox)
            .AllowSpin(false)
            .MinValue(1)
            .MaxValue(32)
            .OnValueCommitted(this, &SMinesweeperWindow::OnSetWidth)
            .OnValueChanged(this, &SMinesweeperWindow::OnSetWidth, ETextCommit::Default)
            .Font(GSlateInfo)
        ]
        
        //Height box
        + SHorizontalBox::Slot()
        .Padding(FMargin(10))
        .AutoWidth()
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Top)
        [
            SNew(STextBlock)
            .Text(GHeightText)
            .Font(GSlateInfo)
        ]
        
        + SHorizontalBox::Slot()
        .Padding(FMargin(10))
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Fill)
        [
            SNew(SNumericEntryBox<int32>)
            .Value(this, &SMinesweeperWindow::GetHeight)
            .MinDesiredValueWidth(GMinDesiredWidthForEntryBox)
            .AllowSpin(false)
            .MinValue(1)
            .MaxValue(32)
            .OnValueCommitted(this, &SMinesweeperWindow::OnSetHeight)
            .OnValueChanged(this, &SMinesweeperWindow::OnSetHeight, ETextCommit::Default)
            .Font(GSlateInfo)
        ]
    ];
    
    ContentArea->AddSlot()
    .HAlign(HAlign_Fill)
    .VAlign(VAlign_Top)
    .Padding(FMargin(0, 100))
    [
        //Number of mines box
        SNew (SHorizontalBox)
        
        + SHorizontalBox::Slot()
        .Padding(FMargin(10))
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Top)
        .AutoWidth()
         [
             SNew(STextBlock)
            .Text(GNumberOfMinesText)
            .Font(GSlateInfo)
         ]
    
        + SHorizontalBox::Slot()
        .Padding(FMargin(10))
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Fill)
         [
             SNew(SNumericEntryBox<int32>)
            .Value(this, &SMinesweeperWindow::GetNumberOfMines)
            .MinDesiredValueWidth(GMinDesiredWidthForEntryBox)
            .AllowSpin(false)
            .MinValue(1)
            .MaxValue(32)
            .OnValueCommitted(this, &SMinesweeperWindow::OnSetNumberOfMines)
            .OnValueChanged(this, &SMinesweeperWindow::OnSetNumberOfMines, ETextCommit::Default)
            .Font(GSlateInfo)
         ]
    ];
    
    ContentArea->AddSlot()
    .HAlign(HAlign_Fill)
    .VAlign(VAlign_Top)
    .Padding(FMargin(0, 150))
    [
        //Button box
        SNew (SHorizontalBox)
        
        + SHorizontalBox::Slot()
        .Padding(FMargin(10))
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Top)
        .AutoWidth()
         [
             SNew(SButton)
            .ButtonStyle(FAppStyle::Get(), "Button")
            .OnClicked(this, &SMinesweeperWindow::OnGenerateNewGridClicked)
            .Cursor(EMouseCursor::Default)
            .Content()
             [
                 SNew(STextBlock)
                .Justification(ETextJustify::Center)
                .Text(GGenerateNewGridText)
                .Font(GSlateInfo)
             ]
        ]
    ];
    
    ContentArea->AddSlot()
    .HAlign(HAlign_Fill)
    .VAlign(VAlign_Top)
    .Padding(FMargin(0, 200))
    [
        //Uniform grid panel
        SNew (SVerticalBox)
        
        + SVerticalBox::Slot()
        .Padding(FMargin(10))
        .VAlign(VAlign_Fill)
         [
             SNew(SBorder)
             .Padding(6)
             .BorderImage(FEditorStyle::GetBrush("ContentBrowser.TileViewTooltip.ContentBorder"))
             [
                 m_UniformGridPanel.ToSharedRef()
             ]
         ]
    ];
}

void SMinesweeperWindow::CreateNewGrid()
{
    check(m_UniformGridPanel.IsValid());
 
    m_NumberOfCellsLeft = m_SelectedHeight * m_SelectedWidth;
    
    m_UniformGridPanel->ClearChildren();
    
    for(auto Column = 0; Column < m_SelectedHeight; ++Column)
    {
        for(auto Row = 0; Row < m_SelectedWidth; ++Row)
        {
            //Should be (Column, Row) but for some reason goes (Row, Column)
            m_UniformGridPanel->AddSlot(Row, Column)
            [
                SNew(SMinesweeperGridSlot)
                .OnGridSlotSelected(this, &SMinesweeperWindow::OnGridPanelClicked)
                .AddMetaData<FSMinesweeperGridSlotMetadata>(FSMinesweeperGridSlotMetadata(Column, Row))
            ];
        }
    }
    
    MarkBombs();
}

TOptional<int32> SMinesweeperWindow::GetWidth() const
{
    return m_SelectedWidth;
}

TOptional<int32> SMinesweeperWindow::GetHeight() const
{
    return m_SelectedHeight;
}

TOptional<int32> SMinesweeperWindow::GetNumberOfMines() const
{
    return m_SelectedNumberOfMines;
}

FLinearColor SMinesweeperWindow::GetNumberColor(const int32 index) const
{
    switch (index)
    {
        case 1: return FColor::Green;
        case 2: return FColor::Orange;
        default: return FColor::Red;
    }
}

void SMinesweeperWindow::MarkBombs()
{
    int32 MineCount = m_SelectedNumberOfMines;
    int32 CellsLeft = m_SelectedWidth * m_SelectedHeight;
    
    if (MineCount > CellsLeft)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to populate bombs because there are more bombs than cells"));
        return;
    }

    int32 Mines = 0;
    if(auto Slots = m_UniformGridPanel->GetChildren())
    {
        const auto GetSlotRef = [&Slots](const int32 Index) -> TSharedRef<SMinesweeperGridSlot>
        {
            return StaticCastSharedRef<SMinesweeperGridSlot>(Slots->GetChildAt(Index));
        };
        
        while (Mines < MineCount)
        {
            const int32 Row = FMath::RandRange(0, m_SelectedWidth-1);
            const int32 Column = FMath::RandRange(0, m_SelectedHeight-1);
            
            int32 Index = m_SelectedHeight * Column + Row;
            if(GetSlotRef(Index)->GetSlotType() == EMinesweeperGridSlotType::Bomb)
            {
                continue;
            }
            
            GetSlotRef(Index)->SetSlotType(EMinesweeperGridSlotType::Bomb);
            Mines++;
        }
    }
}

void SMinesweeperWindow::ExpandRecursively(const int32 Column, const int32 Row)
{
    int32 SlotValue = CountAdjBombs(Column, Row);
    if(SlotValue == 0)
    {
        if(auto Slots = m_UniformGridPanel->GetChildren())
        {
            auto GetSlotRef = [&Slots](const int32 Index) -> TSharedRef<SMinesweeperGridSlot>
            {
                return StaticCastSharedRef<SMinesweeperGridSlot>(Slots->GetChildAt(Index));
            };
            
            const int Index = m_SelectedHeight * Column + Row;
            
            if(IsValidGridPanelIndex(Index) == false ||  GetSlotRef(Index)->GetSlotType() == EMinesweeperGridSlotType::Bomb
               || GetSlotRef(Index)->IsExpanded() == true)
            {
                return;
            }
            
            GetSlotRef(Index)->SetExpanded();
            m_NumberOfCellsLeft -= 1;
            
            ExpandRecursively(Column-1, Row);
            ExpandRecursively(Column+1, Row);
            ExpandRecursively(Column, Row-1);
            ExpandRecursively(Column, Row+1);
        }
        
        return;
    }
    
    SetGridSlotValue(Column, Row, SlotValue);
}

int32 SMinesweeperWindow::CountAdjBombs(const int32 Column, const int32 Row)
{
    int32 BombCount = 0;
    if(auto Slots = m_UniformGridPanel->GetChildren())
    {
        auto GetSlotRef = [&Slots](const int32 Index) -> TSharedRef<SMinesweeperGridSlot>
        {
            return StaticCastSharedRef<SMinesweeperGridSlot>(Slots->GetChildAt(Index));
        };
        
        int32 Index = m_SelectedHeight * (Column - 1) + Row;
        if(IsValidGridPanelIndex(Index) == true && GetSlotRef(Index)->GetSlotType() == EMinesweeperGridSlotType::Bomb)
        {
            BombCount++;
        }
        
        Index = m_SelectedHeight * (Column + 1) + Row;
        if(IsValidGridPanelIndex(Index) == true && GetSlotRef(Index)->GetSlotType() == EMinesweeperGridSlotType::Bomb)
        {
            BombCount++;
        }
        
        Index = m_SelectedHeight * Column + (Row - 1);
        if(IsValidGridPanelIndex(Index) == true && GetSlotRef(Index)->GetSlotType() == EMinesweeperGridSlotType::Bomb)
        {
            BombCount++;
        }
        
        Index = m_SelectedHeight * Column + (Row + 1);
        if(IsValidGridPanelIndex(Index) == true && GetSlotRef(Index)->GetSlotType() == EMinesweeperGridSlotType::Bomb)
        {
            BombCount++;
        }
    }
    
    return BombCount;
}

void SMinesweeperWindow::SetGridSlotValue(const int32 Column, const int32 Row, const int32 SlotValue)
{
    if(auto Slots = m_UniformGridPanel->GetChildren())
    {
        auto GetSlotRef = [&Slots](const int32 Index) -> TSharedRef<SMinesweeperGridSlot>
        {
            return StaticCastSharedRef<SMinesweeperGridSlot>(Slots->GetChildAt(Index));
        };
        
        const int Index = m_SelectedHeight * Column + Row;
        if(IsValidGridPanelIndex(Index) && GetSlotRef(Index)->GetSlotType() != EMinesweeperGridSlotType::Bomb)
        {
            GetSlotRef(Index)->SetSlotValue(SlotValue);
        }
    }
}

void SMinesweeperWindow::MakeAllGridVisible()
{
    if(auto Slots = m_UniformGridPanel->GetChildren())
    {
        const auto GetSlotRef = [&Slots](const int32 Index) -> TSharedRef<SMinesweeperGridSlot>
        {
            return StaticCastSharedRef<SMinesweeperGridSlot>(Slots->GetChildAt(Index));
        };
        
        for(auto Column = 0; Column < m_SelectedHeight; ++Column)
        {
            for(auto Row = 0; Row < m_SelectedWidth; ++Row)
            {
                ExpandRecursively(Column, Row);
                
                const int Index = m_SelectedHeight * Column + Row;
                if(GetSlotRef(Index)->GetSlotType() == EMinesweeperGridSlotType::Bomb)
                {
                    GetSlotRef(Index)->SetSlotTextBasedOnCurrentType();
                }
            }
        }
    }
}

bool SMinesweeperWindow::IsValidGridPanelIndex(int32 Index) const
{
    return m_SelectedWidth * m_SelectedHeight > Index && Index >= 0;
}

// ------------------------------------------------------------------------
// Protectred class functions (Callbacks)
// ------------------------------------------------------------------------

void SMinesweeperWindow::OnSetWidth(int32 InNewValue, ETextCommit::Type InCommitType)
{
    if(InCommitType == ETextCommit::OnCleared)
    {
        return;
    }

    m_SelectedWidth = InNewValue;
}

void SMinesweeperWindow::OnSetHeight(int32 InNewValue, ETextCommit::Type InCommitType)
{
    if(InCommitType == ETextCommit::OnCleared)
    {
        return;
    }

    m_SelectedHeight = InNewValue;
}

void SMinesweeperWindow::OnSetNumberOfMines(int32 InNewValue, ETextCommit::Type InCommitType)
{
    if(InCommitType == ETextCommit::OnCleared)
    {
        return;
    }

    m_SelectedNumberOfMines = InNewValue;
}

FReply SMinesweeperWindow::OnGenerateNewGridClicked()
{
    if (m_SelectedNumberOfMines > m_SelectedWidth * m_SelectedHeight)
    {
        //FSlateNotificationManager::Get().AddNotification(FNotificationInfo(GCanNotHaveMoreMinesText));
        FMessageDialog::Open(EAppMsgType::Ok, GCanNotHaveMoreMinesText);
    }
    else
    {
        CreateNewGrid();
    }
    
    return FReply::Handled();
}

void SMinesweeperWindow::OnGridPanelClicked(int32 Column, int32 Row)
{
    if(auto Slots = m_UniformGridPanel->GetChildren())
    {
        const auto GetSlotRef = [&Slots](const int32 Index) -> TSharedRef<SMinesweeperGridSlot>
        {
            return StaticCastSharedRef<SMinesweeperGridSlot>(Slots->GetChildAt(Index));
        };
        
        const int32 Index = m_SelectedHeight * Column + Row;
        if(IsValidGridPanelIndex(Index) && GetSlotRef(Index)->GetSlotType() == EMinesweeperGridSlotType::Bomb)
        {
            FMessageDialog::Open(EAppMsgType::Ok, GLostGameText);
            MakeAllGridVisible();
            return;
        }
        
        ExpandRecursively(Column, Row);
        if(m_NumberOfCellsLeft == m_SelectedNumberOfMines)
        {
            FMessageDialog::Open(EAppMsgType::Ok, GWonGameText);
            MakeAllGridVisible();
        }
    }
}

#undef LOCTEXT_NAMESPACE
