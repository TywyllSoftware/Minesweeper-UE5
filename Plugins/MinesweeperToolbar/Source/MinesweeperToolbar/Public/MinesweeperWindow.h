//
//  MinesweeperWindow.hpp
//  Minesweeper_Index
//
//  Created by Tomas Mikalauskas on 09/05/2022.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//
#pragma once

//Engine Includes
#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Engine/Texture2D.h"

class SMinesweeperWindow: public SCompoundWidget
{
public:
    // ------------------------------------------------------------------------
    // Public class functions
    // ------------------------------------------------------------------------
    
    SLATE_BEGIN_ARGS(SMinesweeperWindow) {}
    SLATE_END_ARGS()

    // Constructs this widget with InArgs
    void Construct(const FArguments& InArgs, UTexture2D* Texture);

    virtual ~SMinesweeperWindow();

    // Show the dialog, returns true if successfully extracted sprites
    static bool ShowWindow(UTexture2D* SourceTexture);

protected:
    
    // ------------------------------------------------------------------------
    // Protected class functions
    // ------------------------------------------------------------------------
    
    /*
        @brief Creates  minesweeper window  layout
     */
    void CreatePanelArea();
    
    /*
        @brief Creates minesweeper grid layout
     */
    void CreateNewGrid();
    
    /*
        @return Current selected width of grid panel
     */
    TOptional<int32> GetWidth() const;
    
    /*
        @return Current selected height of grid panel
     */
    TOptional<int32> GetHeight() const;
    
    /*
        @return Current selected Number Of Mines
     */
    TOptional<int32> GetNumberOfMines() const;
        
    /*
        @param index
        @return
     */
    FLinearColor GetNumberColor(const int32 index) const;
    
    /*
        @brief Callback when clicked on grid cell
     
        @param Column
        @param Row
     */
    void OnGridPanelClicked(int32 Column, int32 Row);
    
    
    void MarkBombs();
    
    /*
        @param Column
        @param Row
     */
    void ExpandRecursively(const int32 Column, const int32 Row);
    
    /*
        @param Column
        @param Row
     
        @return
     */
    int32 CountAdjBombs(const int32 Column, const int32 Row);
    
    /*
        @param Column
        @param Rrow
     */
    void SetGridSlotValue(const int32 Column, const int32 Row, const int32 SlotValue);
    
    void MakeAllGridVisible();
    
    /*
        @brief Checks if given index within the bounds of created uniform grid
     
        @param Index
     */
    bool IsValidGridPanelIndex(int32 Index) const;
    
protected:
    
    // ------------------------------------------------------------------------
    // Protectred class functions (Callbacks)
    // ------------------------------------------------------------------------
    
    /*
     @brief Callback called when new number is entered into input box
     
        @param InNewText
        @param InCommitType
     */
    void OnSetWidth(int32 InNewValue, ETextCommit::Type InCommitType);
    
    /*
        @brief Callback called when new number is entered into input box
     
        @param InNewText
        @param InCommitType
     */
    void OnSetHeight(int32 InNewValue, ETextCommit::Type InCommitType);
        
    /*
        @brief Callback called when new number is entered into input box
     
        @param InNewText
        @param InCommitType
     */
    void OnSetNumberOfMines(int32 InNewValue, ETextCommit::Type InCommitType);
    
    /*
        @brief Callback called when clicked on Generate new grid
     
        @return
     */    
    FReply OnGenerateNewGridClicked();

    
private:

    int32 m_SelectedWidth = 0;
    int32 m_SelectedHeight = 0;
    int32 m_SelectedNumberOfMines = 0;
    int32 m_NumberOfCellsLeft = 0;
    
    // Main panel area widget
    TSharedPtr<SBorder>           m_PanelArea = nullptr;
    
    //Minesweep grid
    TSharedPtr<SUniformGridPanel> m_UniformGridPanel = nullptr;
};
