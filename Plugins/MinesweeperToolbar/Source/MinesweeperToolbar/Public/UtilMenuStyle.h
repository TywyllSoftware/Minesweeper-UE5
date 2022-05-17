//
//  UtilMenuStyle.hpp
//  Minesweeper_Index
//
//  Created by Tomas Mikalauskas on 09/05/2022.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#pragma once
#include "SlateBasics.h"


class FUtilMenuStyle
{
public:

    static void Initialize();

    static void Shutdown();

    static void ReloadTextures();

    static const ISlateStyle& Get();

    static FName GetStyleSetName();

private:

    static TSharedRef<class FSlateStyleSet> Create();

private:

    static TSharedPtr<class FSlateStyleSet> StyleInstance;
};
