// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperToolbarModule.h"

//Editor includes
#include "UtilMenuStyle.h"
#include "UtilMenuCommands.h"
#include "MinesweeperWindow.h"

//Engine Includes
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "FMinesweeperToolbarModule"

void FMinesweeperToolbarModule::StartupModule()
{
    FUtilMenuStyle::Initialize();
    FUtilMenuCommands::Register();
    
    m_UtilMenuCommandList = MakeShareable(new FUICommandList);
    
    m_UtilMenuCommandList->MapAction(
        FUtilMenuCommands::Get().PlayNewGame,
        FExecuteAction::CreateRaw(this, &FMinesweeperToolbarModule::PlayNewGameClicked),
        FCanExecuteAction());
                                   
    m_MyExtender = MakeShareable(new FExtender);
    m_MyExtender->AddToolBarExtension("Content", EExtensionHook::After, NULL, FToolBarExtensionDelegate::CreateRaw(this, &FMinesweeperToolbarModule::AddToolbarExtension));
    
    FLevelEditorModule& levelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    levelEditorModule.GetToolBarExtensibilityManager()->AddExtender(m_MyExtender);
}

void FMinesweeperToolbarModule::ShutdownModule()
{
    FLevelEditorModule& levelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    levelEditorModule.GetToolBarExtensibilityManager()->RemoveExtender(m_MyExtender);
    
    FUtilMenuCommands::Unregister();
    FUtilMenuStyle::Shutdown();
}

void FMinesweeperToolbarModule::AddToolbarExtension(FToolBarBuilder& builder)
{
    FUIAction UtilMenuAction;

    builder.AddComboButton(
        UtilMenuAction,
        FOnGetContent::CreateStatic(&FMinesweeperToolbarModule::GenerateUtilityMenu, m_UtilMenuCommandList.ToSharedRef()),
        LOCTEXT("MineSweeperToolbarMenu_Label", "Minesweeper"),
        LOCTEXT("MineSweeperToolbarMenu_Tooltip", "Minesweeper Toolbar"),
        FSlateIcon(FUtilMenuStyle::GetStyleSetName(), "MinesweeperToolbar.UtilityMenu")
    );
}

TSharedRef<SWidget> FMinesweeperToolbarModule::GenerateUtilityMenu(TSharedRef<FUICommandList> InCommandList)
{
    FMenuBuilder menuBuilder(true, InCommandList);
    menuBuilder.BeginSection("Game");
    {
        menuBuilder.AddMenuEntry(FUtilMenuCommands::Get().PlayNewGame);
    }
    
    menuBuilder.EndSection();
    
    return menuBuilder.MakeWidget();
}

void FMinesweeperToolbarModule::PlayNewGameClicked()
{
    SMinesweeperWindow::ShowWindow(nullptr);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMinesweeperToolbarModule, MinesweeperToolbarEditorMode)
