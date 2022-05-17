//
//  UtilMenuCommands.cpp
//  Minesweeper_Index
//
//  Created by Tomas Mikalauskas on 09/05/2022.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//
#include "UtilMenuCommands.h"

//Engine Includes
#include "LevelEditor.h"

FUtilMenuCommands::FUtilMenuCommands()
    : TCommands<FUtilMenuCommands>(TEXT("MinesweeperToolbarPlugin"), NSLOCTEXT("Contexts", "MinesweeperToolbarPlugin", "MinesweeperToolbarPlugin Plugin"), NAME_None, FEditorStyle::GetStyleSetName())
{

}

#ifdef WITH_EDITOR

#define LOCTEXT_NAMESPACE ""

void FUtilMenuCommands::RegisterCommands()
{
    UI_COMMAND(PlayNewGame, "Play new game", "Opens new game window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE

#endif
