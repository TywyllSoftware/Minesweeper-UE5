//
//  UtilMenuCommands.hpp
//  Minesweeper_Index
//
//  Created by Tomas Mikalauskas on 09/05/2022.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//
#pragma once

#include "SlateBasics.h"

class FUtilMenuCommands : public TCommands<FUtilMenuCommands>
{
public:

    FUtilMenuCommands();

#if WITH_EDITOR
    virtual void RegisterCommands() override;
#endif

    TSharedPtr<FUICommandInfo> PlayNewGame;
};
